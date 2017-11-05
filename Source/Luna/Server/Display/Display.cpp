#include "../../../../Include/Luna/Server/Display/Display.hpp"

using namespace Luna::Common;
using namespace Luna::Server;

std::vector<uint32_t> Display::fUsedCRTCs;

//==============================================================================
Display::Display(int fd) : fConnectorID(0), fEncoderID(0), fCRTCID(0), fFD(fd),
  fRendering(false), fMidBufReady(false), fFrontBufReady(false),
  fSavedCRTC(nullptr, nullptr)
{
  LUNA_TRACE_FUNCTION();
}

//==============================================================================
Display::~Display()
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
}

//==============================================================================
uint32_t Display::connectorID() const
{
  LUNA_TRACE_FUNCTION();
  return fConnectorID;
}

//==============================================================================
void Display::configure(int fd, drmModeConnector * conn, drmModeRes * res,
                        const Settings * settings)
{
  LUNA_TRACE_FUNCTION();

  // Make sure the rendering is disabled.
  stopRendering();

  LUNA_LOG_DEBUG("Current connector id: " << fConnectorID << " new id: " <<
                 conn->connector_id << ".");

  // Make sure it is either a new configuration, or the connector id's match
  // if it's a reconfiguration.
  if(fConnectorID != 0 && fConnectorID != conn->connector_id)
  {
    LUNA_THROW_RUNTIME_ERROR("Connect ID missmatch for reconfiguration, " <<
                             "expected: " << fConnectorID << ", got: " <<
                             conn->connector_id << ".");
  }

  // Record the connector ID for future reference.
  fConnectorID = conn->connector_id;

  // Make sure the connector has atleast on supported mode.
  if(conn->count_modes == 0)
  {
    LUNA_THROW_RUNTIME_ERROR("Connector " << conn->connector_id <<
                             " does not have any supported modes.");
  }

  LUNA_LOG_INFO("Connector " << conn->connector_id << " supports " <<
                conn->count_modes << " modes.");

  // Check the best mode to use for the display.
  fActiveMode = getBestMode(conn->modes, conn->count_modes, settings);

  LUNA_LOG_INFO("Selected mode for connector " << conn->connector_id <<
                " is " << fActiveMode.hdisplay << " x " <<
                fActiveMode.vdisplay << " @ " << fActiveMode.vrefresh << "Hz.");

  // Setup a suitable Encoder and CRT Controller.
  setupEncoderAndCRTC(fd, conn, res);

  // Setup the buffers.
  LUNA_LOG_DEBUG("Creating front buffer.");
  fFrontBuffer = std::make_unique<FrameBuffer>(fd, fActiveMode.hdisplay,
                                               fActiveMode.vdisplay, 32);

  LUNA_LOG_DEBUG("Creating middle buffer.");
  fMiddleBuffer = std::make_unique<FrameBuffer>(fd, fActiveMode.hdisplay,
                                                fActiveMode.vdisplay, 32);

  LUNA_LOG_DEBUG("Creating back buffer.");
  fBackBuffer = std::make_unique<FrameBuffer>(fd, fActiveMode.hdisplay,
                                              fActiveMode.vdisplay, 32);
}
//==============================================================================
bool Display::isEncoderAndCRTCValid(int fd, drmModeConnector * conn)
{
  LUNA_TRACE_FUNCTION();

  // The status of the function.
  bool status = false;

  LUNA_LOG_DEBUG("Checking if existing encoder and crtc is valid.");

  LUNA_LOG_DEBUG("Current encoder id: " << fEncoderID << " existing id: " <<
                 conn->encoder_id << ".");

  // Check if a valid Encoder is allready attached to the connector.
  if(fEncoderID > 0 && conn->encoder_id == fEncoderID)
  {
    // Get the handle to the encoder.
    std::unique_ptr<drmModeEncoder, decltype(&drmModeFreeEncoder)>
        encoder(drmModeGetEncoder(fd, fEncoderID), drmModeFreeEncoder);

    LUNA_LOG_DEBUG("Current crtc id: " << fCRTCID << " existing id: " <<
                   encoder->crtc_id << ".");

    // Check if the CRT Controller is valid.
    if(encoder && fCRTCID > 0 && encoder->crtc_id == fCRTCID)
    {
      // The Encoder and CRTC is valid.
      status = true;
    }
  }

  // Return the status of the function.
  return status;
}

//==============================================================================
void Display::setupEncoderAndCRTC(int fd, drmModeConnector * conn,
                                  drmModeRes * res)
{
  LUNA_TRACE_FUNCTION();

  // Before trying to find a suitabled Encoder and CRTC, check if the current
  // ones are valid. (This should be true for most reconfigurations.)
  if(isEncoderAndCRTCValid(fd, conn))
  {
    LUNA_LOG_DEBUG("Existing encoder and crtc is valid.");

    // Nothing left to do, just return.
    return;
  }

  LUNA_LOG_DEBUG("Existing encoder and crctc is not valid.");

  // Check if an ecoder is allready attached to the connector.
  if(conn->encoder_id)
  {
    LUNA_LOG_DEBUG("Checking if currently attached encoder (" <<
                   conn->encoder_id << ") is suitable.");

    // Get the handle to the encoder.
    std::unique_ptr<drmModeEncoder, decltype(&drmModeFreeEncoder)>
        encoder(drmModeGetEncoder(fd, conn->encoder_id),
                drmModeFreeEncoder);

    // Check if a valid encoder was returned.
    if(encoder)
    {
      LUNA_LOG_DEBUG("Checking if currently attached crtc (" <<
                     encoder->crtc_id << ") is suitable.");

      // Check if the CRT Controller is allready in use.
      if(std::find(fUsedCRTCs.begin(), fUsedCRTCs.end(), encoder->crtc_id) ==
         fUsedCRTCs.end())
      {
        // Set the encoder and crtc id.
        fEncoderID = conn->encoder_id;
        fCRTCID = encoder->crtc_id;


        LUNA_LOG_DEBUG("The encoder (" << fEncoderID << ") and crtc (" <<
                       fCRTCID << ") pair is suitable.");

        // The encoder and CRTC is both valid and allready connected. Nothing
        // else left to do.
        return;
      }
    }
  }

  // Not suitable Encoder or CRTC has been found yet. Iterate through all the
  // possible encoders and CRTCs and find one not allready in use.
  for(int i = 0; i < conn->count_encoders; i++)
  {
    // Get the handle to the encoder.
    std::unique_ptr<drmModeEncoder, decltype(&drmModeFreeEncoder)>
        encoder(drmModeGetEncoder(fd, conn->encoder_id),
                drmModeFreeEncoder);

    // Check if the encoder was retrieved.
    if(!encoder)
    {
      LUNA_LOG_ERROR("Failed to retrieve encoder " << i << " because: " <<
                     strerror(errno));
      continue;
    }

    // Iterate trhough all gloab CRTCs.
    for(int j = 0; j < res->count_crtcs; ++j)
    {
      // Check whether this CRTC works with the encoder. This crazyness brought
      // to you by:
      // https://dvdhrm.wordpress.com/2012/09/13/linux-drm-mode-setting-api/
      if (!(encoder->possible_crtcs & (1 << j)))
      {
        continue;
      }

      // Check if the CRTC is allready in use.
      if(std::find(fUsedCRTCs.begin(), fUsedCRTCs.end(), res->crtcs[j]) ==
         fUsedCRTCs.end())
      {
        // Save the Encoder and CRTC ID.
        fEncoderID = encoder->encoder_id;
        fCRTCID = res->crtcs[j];

        // Nothing left to do.
        return;
      }
    }
  }

  LUNA_THROW_RUNTIME_ERROR("Unable to find a suitable CRTC for the connector.");
}

//==============================================================================
uint32_t Display::physicalWidth() const
{
  LUNA_TRACE_FUNCTION();
  return fWidthMM;
}

//==============================================================================
uint32_t Display::physicalHeight() const
{
  LUNA_TRACE_FUNCTION();
  return fHeightMM;
}

//==============================================================================
 float Display::dpmmX() const
{
  LUNA_TRACE_FUNCTION();

  // TODO - DECIDE WHAT TO DO HERE.
  // The calculated dpmm.
  float dpmm = 0; //static_cast<float>(fActiveMode.fWidthPx);

  // Check for a possible divide-by-zero error.
  if(fWidthMM > 0)
  {
    // Finish the dpmm calculation.
    dpmm /= static_cast<float>(fWidthMM);
  }
  else
  {
    LUNA_LOG_WARN("The display (" << fConnectorID << ") has no valid " <<
                  "physical width information.");
  }

  // Return the dpmm.
  return dpmm;
}

//==============================================================================
float Display::dpmmY() const
{
  LUNA_TRACE_FUNCTION();

  // TODO - DECIDE WHAT TO DO HERE.
  // The calculated dpmm.
  float dpmm = 0; //static_cast<float>(fActiveMode.fHeightPx);

  // Check for a possible divide-by-zero error.
  if(fWidthMM > 0)
  {
    // Finish the dpmm calculation.
    dpmm /= static_cast<float>(fHeightMM);
  }
  else
  {
    LUNA_LOG_WARN("The display (" << fConnectorID << ") has no valid " <<
                  "physical height information.");
  }

  // Return the dpmm.
  return dpmm;
}

//==============================================================================
void Display::setMode(int fd/*, drmModeConnector * conn*/)
{
  LUNA_TRACE_FUNCTION();

  // Check if this is the first mode change. If it is, then save the mode so
  // it can be recovered when the application terminates.
  if(!fSavedCRTC)
  {
    LUNA_LOG_DEBUG("Saving current crtc mode.");
    fSavedCRTC = std::unique_ptr<drmModeCrtc, decltype(&drmModeFreeCrtc)>
        (drmModeGetCrtc(fd, fCRTCID), drmModeFreeCrtc);
  }

  LUNA_LOG_DEBUG("Setting desired crtc mode.");
  if(drmModeSetCrtc(fd, fCRTCID, fFrontBuffer->fID, 0, 0, &fConnectorID, 1,
                 &fActiveMode) < 0)
  {
    LUNA_THROW_RUNTIME_ERROR("Failed to change CRTC mode.");
  }

  LUNA_LOG_DEBUG("Starting the rendering thread.");
  startRendering();
}

//==============================================================================
drmModeModeInfo Display::getBestMode(drmModeModeInfoPtr modes,
  uint32_t numModes, const Settings * settings)
{
  LUNA_UNUSED_PARAM(numModes);
  LUNA_UNUSED_PARAM(settings);

  // TODO - Implement this.
  return modes[0];
}

//==============================================================================
void Display::fill(uint8_t r, uint8_t g, uint8_t b)
{
  LUNA_TRACE_FUNCTION();
  uint8_t * pixel = fBackBuffer->pixels();

  for(int row = 0; row < fActiveMode.vdisplay; row++)
  {
    for(int col = 0; col < fActiveMode.hdisplay*4; col += 4)
    {
      pixel[row * fActiveMode.hdisplay * 4 + col * 4 + 1] = r;
      pixel[row * fActiveMode.hdisplay * 4 + col * 4 + 2] = g;
      pixel[row * fActiveMode.hdisplay * 4 + col * 4 + 3] = b;
    }
  }
}

//==============================================================================
void Display::swapBuffers()
{
  LUNA_TRACE_FUNCTION();

  // Lock the protection mutex.
  std::lock_guard<std::mutex> lock(fMidBuffLock);

  // Swap the back and middle buffer.
  fBackBuffer.swap(fMiddleBuffer);

  // Indicate that a new frame is available.
  fMidBufReady = true;
}

//==============================================================================
void Display::render()
{
  LUNA_TRACE_FUNCTION();

  // Keep looping till stopRendering() is called.
  while(fRendering)
  {
    // Check if the buffer should be flipped.
    if(fMidBufReady)
    {
      LUNA_LOG_DEBUG("Swapping Front <-> Middle buffers.");

      // Lock the protection mutex.
      std::lock_guard<std::mutex> lock(fMidBuffLock);

      // Swap the pointers.
      fFrontBuffer.swap(fMiddleBuffer);

      // Mark the front buffer as ready to be rendered.
      fFrontBufReady = true;
    }

    // Check if the front buffer should be rendered.
    if(fFrontBufReady)
    {
      LUNA_LOG_DEBUG("Flipping page.");

      drmVBlank vbl;
      vbl.request.type = DRM_VBLANK_RELATIVE;
      vbl.request.sequence = 1;

      // Wait for a VBlank event.
      drmWaitVBlank(fFD, &vbl);

      // Flip the page.
      drmModePageFlip(fFD, fCRTCID, fFrontBuffer->id(),
                      DRM_MODE_PAGE_FLIP_EVENT, nullptr);

      // Mark the front buffer as not ready (don't paint unless we need too).
      fFrontBufReady = false;
    }
  }

  // Wait for a VBlank event so we don't inadvertedly set crtc in the middle of
  // a page flip.
  drmVBlank vbl;
  vbl.request.type = DRM_VBLANK_RELATIVE;
  vbl.request.sequence = 1;

  LUNA_LOG_DEBUG("Waiting for last vblank event.");
  drmWaitVBlank(fFD, &vbl);
}

//==============================================================================
void Display::startRendering()
{
  LUNA_TRACE_FUNCTION();

  // Dont start it again if it's allready rederning.
  if(!fRendering)
  {
    LUNA_LOG_DEBUG("Starting render thread.");
    // Set the rendering flag before starting the thread.
    fRendering = true;

    // Create the thread.
    fRenderThread = std::make_unique<std::thread>(&Display::render, this);
  }
  else
  {
    LUNA_LOG_DEBUG("Rendering thread allready running.");
  }
}

//==============================================================================
void Display::stopRendering()
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

//##############################################################################
//                                DUMB BUFFER
//##############################################################################
Display::DumbBuffer::DumbBuffer(int fd, uint32_t width, uint32_t height,
                                uint32_t bpp) : fResult(-1), fFD(fd)
{
  LUNA_TRACE_FUNCTION();

  // Clear the memory.
  memset(&fBuffer, 0, sizeof(fBuffer));

  // Set the desired fields.
  fBuffer.width = width;
  fBuffer.height = height;
  fBuffer.bpp = bpp;

  LUNA_LOG_DEBUG("Creating dumb buffer.");
  fResult = drmIoctl(fFD, DRM_IOCTL_MODE_CREATE_DUMB, &fBuffer);
}

//==============================================================================
Display::DumbBuffer::~DumbBuffer()
{
  LUNA_TRACE_FUNCTION();

  // Check if the buffer was created.
  if(fResult >= 0)
  {
    // Setup the delete buffer.
    struct drm_mode_destroy_dumb deleteBuffer;
    memset(&deleteBuffer, 0, sizeof(deleteBuffer));
    deleteBuffer.handle = fBuffer.handle;

    LUNA_LOG_DEBUG("Destroying dumb buffer.");

    // Delete the buffer.
    drmIoctl(fFD, DRM_IOCTL_MODE_DESTROY_DUMB, &deleteBuffer);
  }
}

//==============================================================================
uint32_t Display::DumbBuffer::width() const
{
  LUNA_TRACE_FUNCTION();
  return fBuffer.width;
}

//==============================================================================
uint32_t Display::DumbBuffer::height() const
{
  LUNA_TRACE_FUNCTION();
  return fBuffer.height;
}

//==============================================================================
uint32_t Display::DumbBuffer::bpp() const
{
  LUNA_TRACE_FUNCTION();
  return fBuffer.bpp;
}

//==============================================================================
uint32_t Display::DumbBuffer::stride() const
{
  LUNA_TRACE_FUNCTION();
  return fBuffer.pitch;
}

//==============================================================================
uint32_t Display::DumbBuffer::handle() const
{
  LUNA_TRACE_FUNCTION();
  return fBuffer.handle;
}

//==============================================================================
uint32_t Display::DumbBuffer::size() const
{
  LUNA_TRACE_FUNCTION();
  return fBuffer.size;
}

//##############################################################################
//                                FRAME BUFFER
//##############################################################################
Display::FrameBuffer::FrameBuffer(int fd, uint32_t width, uint32_t height,
  uint32_t bpp) : fFD(fd),
  fDumbBuffer(std::make_unique<DumbBuffer>(fd, width, height, bpp))
{
  LUNA_TRACE_FUNCTION();

  LUNA_LOG_DEBUG("Creating Frame Buffer object.");
  fResult = drmModeAddFB(fFD,
                         fDumbBuffer->width(),
                         fDumbBuffer->height(),
                         24,
                         fDumbBuffer->bpp(),
                         fDumbBuffer->stride(),
                         fDumbBuffer->handle(),
                         &fID);

  LUNA_LOG_DEBUG("Preparing buffer for memory map.");

  struct drm_mode_map_dumb mreq;
  memset(&mreq, 0, sizeof(mreq));
  mreq.handle = fDumbBuffer->handle();

  if(drmIoctl(fd, DRM_IOCTL_MODE_MAP_DUMB, &mreq) < 0)
  {
    LUNA_THROW_RUNTIME_ERROR("Failed to map dump buffer because: " <<
                             strerror(errno))
  }

  LUNA_LOG_DEBUG("Performing memory map.");
  fPixelMap = static_cast<uint8_t*>(mmap(nullptr, fDumbBuffer->size(),
                                         PROT_READ | PROT_WRITE,
                   MAP_SHARED, fd, mreq.offset));

  if(fPixelMap == MAP_FAILED)
  {
    LUNA_THROW_RUNTIME_ERROR("Failed to map memory.");
  }
}

//==============================================================================
Display::FrameBuffer::~FrameBuffer()
{
  LUNA_TRACE_FUNCTION();
  if(fResult >= 0)
  {
    drmModeRmFB(fFD, fID);
  }
}

//==============================================================================
uint32_t Display::FrameBuffer::id() const
{
  LUNA_TRACE_FUNCTION();
  return fID;
}

//==============================================================================
uint8_t * Display::FrameBuffer::pixels()
{
  LUNA_TRACE_FUNCTION();
  return fPixelMap;
}
