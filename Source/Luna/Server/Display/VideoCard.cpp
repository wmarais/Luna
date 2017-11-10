#include "../../../../Include/Luna/Server.hpp"

using namespace Luna::Common;
using namespace Luna::Server;

//==============================================================================
VideoCard::VideoCard(const std::string & devNode) : fFD(-1), fRendering(false)
{
  LUNA_TRACE_FUNCTION();

  // Open the video card.
  fFD = open(devNode.c_str(), O_RDWR);

  // Read the current file flags.
  int flags = fcntl(fFD, F_GETFD);

  // set FD_CLOEXEC flag.
  if (flags < 0 || fcntl(fFD, F_SETFD, flags | FD_CLOEXEC) < 0)
  {
    LUNA_LOG_ERROR("Failed to set FD_CLOEXEC flag.");
  }

  // Check if the drm device support dumb buffers.
  uint64_t hasDumbBuffs = 0;
  if (drmGetCap(fFD, DRM_CAP_DUMB_BUFFER, &hasDumbBuffs) < 0 ||
      hasDumbBuffs == 0)
  {
    LUNA_THROW_RUNTIME_ERROR("Device does not support dumb buffers!");
  }
}

//==============================================================================
VideoCard::~VideoCard()
{
  LUNA_TRACE_FUNCTION();

  // Check if the rendering thread is running.
  if(fRendering)
  {
    // Tell the thread to stop.
    fRendering = false;

    // Wait for the thread to join.
    fRenderThread->join();
  }

  // Close the file descritor.
  if(fFD >= 0)
  {
    close(fFD);
    fFD = -1;
  }
}

//==============================================================================
void VideoCard::startRendering()
{
  LUNA_TRACE_FUNCTION();

  // Dont start it again if it's allready rederning.
  if(!fRendering)
  {
    LUNA_LOG_DEBUG("Starting render thread.");
    // Set the rendering flag before starting the thread.
    fRendering = true;

    // Create the thread.
    fRenderThread = std::make_unique<std::thread>(&VideoCard::render, this);
  }
  else
  {
    LUNA_LOG_DEBUG("Rendering thread allready running.");
  }
}

//==============================================================================
void VideoCard::stopRendering()
{
  LUNA_TRACE_FUNCTION();

  if(fRenderThread)
  {
    LUNA_LOG_DEBUG("Stopping rendering thread.");

    // Set rendering to false.
    fRendering = false;

    // wait for the thread to join.
    fRenderThread->join();
  }
  else
  {
    LUNA_LOG_DEBUG("Rendering thread allready stopped.");
  }
}

//==============================================================================
void VideoCard::render()
{
  LUNA_TRACE_FUNCTION();

  // Set the file descriptor to monitor.
  fd_set fds;
  FD_ZERO(&fds);
  FD_SET(fFD, &fds);

  // Setup the event structure.
  drmEventContext drmEvent;
  memset(&drmEvent, 0, sizeof(drmEvent));
  drmEvent.version = 2;
  drmEvent.page_flip_handler = Display::pageFlipEvent;

  // Setup the timeout structure. Clear all the fields to let the select()
  // function time out immediately.
  struct timeval timeOut;
  memset(&timeOut, 0, sizeof(timeOut));
  

  LUNA_LOG_DEBUG("Entering render loop.");

  // Keep looping till stopRendering() is called.
  while(fRendering)
  {
    LUNA_LOG_DEBUG("Rendering displays.");
    // Render each display.
    for(auto & display : fDisplays)
    {
      // Render the display.
      display.second->render(fFD);
    }
    
    // Check if any vblank events occured.
    int ret = select(fFD+1, &fds, nullptr, nullptr, nullptr);

    if(ret < 0)
    {
      LUNA_LOG_FATAL_ERROR("select() failed because: " << strerror(errno));
      exit(-1);
    }
    else if(FD_ISSET(fFD, &fds))
    {
      if(drmHandleEvent(fFD, &drmEvent) < 0)
      {
        LUNA_LOG_FATAL_ERROR("drmHandleEvent() failed because: " << strerror(errno));
        exit(-1);
      }
    }

    // Yield the thread so as not to hog the CPU.
    std::this_thread::yield();
  }

  LUNA_LOG_DEBUG("Exiting render loop.");

}

//==============================================================================
void VideoCard::configure(const Settings * settings)
{
  LUNA_TRACE_FUNCTION();

  LUNA_LOG_DEBUG("Stopping the rendering thread.");
  stopRendering();

  LUNA_LOG_DEBUG("Retrieving DRM resources.");
  std::unique_ptr<drmModeRes, decltype(&drmModeFreeResources)>
      resources(drmModeGetResources(fFD), drmModeFreeResources);

  // Check if the drm resources were retrieved.
  if(!resources)
  {
    LUNA_THROW_RUNTIME_ERROR("Failed to retrieve DRM resources beacause: "
                             << strerror(errno));
  }

  LUNA_LOG_DEBUG("Iterating through all the connectors.");
  for(int i = 0; i < resources->count_connectors; ++i)
  {
    LUNA_LOG_DEBUG("Retrieving connector " << i << " information.");
    std::unique_ptr<drmModeConnector, decltype(&drmModeFreeConnector)>
        connector(drmModeGetConnector(fFD, resources->connectors[i]),
                  drmModeFreeConnector);

    // Try to find the connector in the display list.
    auto iter = fDisplays.find(i);

    // Validate the connector.
    if(!connector)
    {
      // This is a non fatal failure.
      LUNA_LOG_ERROR("Failed to retrieve DRM connector " << i <<
                     "'s information because: " << strerror(errno));

      // Check if a display was connected to the conenctor.
      if(iter != fDisplays.end())
      {
        LUNA_LOG_INFO("Removing display from connector " << i << ".");
        fDisplays.erase(iter);
      }

      // Continue parsing the next conenctor.
      continue;
    }

    LUNA_LOG_DEBUG("Checking if a display is connected to the connector " <<
                   i << ".");
    if(connector->connection != DRM_MODE_CONNECTED)
    {
      LUNA_LOG_DEBUG("Connector " << i << " is not used.");

      // Check if a display was connected to the conenctor.
      if(iter != fDisplays.end())
      {
        LUNA_LOG_INFO("Removing display from connector " << i << ".");
        fDisplays.erase(iter);
      }

      // Continue parsing the next connector.
      continue;
    }

    LUNA_LOG_DEBUG("Checking if the connected display has atleast one valid " <<
                   "mode.");
    if(connector->count_modes == 0)
    {
      // We can't conclusively say if this is an error, and it certainly should
      // not be fatal, however we should warn the user of this strange behavior.
      LUNA_LOG_WARN("The display attached to connector " << i << " has no " <<
                    "valid modes.");
    }

    // Create the display.
    std::unique_ptr<Display> display = std::make_unique<Display>(fFD);

    // COnfigure the display.
    display->configure(fFD, connector.get(), resources.get(), settings);

    // Add it to the list of displays.
    fDisplays[i] = std::move(display);
  }

  // Set all the modes.
  setModes();

  LUNA_LOG_DEBUG("Starting the rendering thread.");
  startRendering();
}

//==============================================================================
void VideoCard::setModes()
{
  // Set all the display modes.
  for(std::map<int, std::unique_ptr<Display>>::iterator iter = fDisplays.begin();
      iter != fDisplays.end(); iter++)
  {
    iter->second->setMode(fFD);
  }
}

//==============================================================================
void VideoCard::renderTest()
{
  // Cycle through red..
  for(int i = 0; i < 255; i+=5)
  {
    // Set all the display modes.
    for(std::map<int, std::unique_ptr<Display>>::iterator iter =
        fDisplays.begin(); iter != fDisplays.end(); iter++)
    {
      iter->second->fill(i, 0, 0);

      // Swap the buffers.
      iter->second->swapBuffers();
    }
  }

  // Cycle through red.
  for(int i = 0; i < 255; i+=5)
  {
    // Set all the display modes.
    for(std::map<int, std::unique_ptr<Display>>::iterator iter =
        fDisplays.begin(); iter != fDisplays.end(); iter++)
    {
      iter->second->fill(0, i, 0);

      // Swap the buffers.
      iter->second->swapBuffers();
    }
  }

  // Cycle through blue.
  for(int i = 0; i < 255; i+=5)
  {
    // Set all the display modes.
    for(std::map<int, std::unique_ptr<Display>>::iterator iter =
        fDisplays.begin(); iter != fDisplays.end(); iter++)
    {
      iter->second->fill(0, 0, i);

      // Swap the buffers.
      iter->second->swapBuffers();
    }
  }
}
