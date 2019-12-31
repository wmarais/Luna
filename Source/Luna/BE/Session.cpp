#include <png++/png.hpp>

#include "../../../Include/Luna/BE/Session.hpp"
#include "../../../Include/Luna/Common/Directory.hpp"
#include "../../../Include/Luna/Common/FrameBuffer.hpp"
#include "../../../Include/Luna/Common/Network/UnixSocketClient.hpp"

using namespace Luna::BE;

/******************************************************************************/
Session::Session()
{
  /* Create the server socket. */
  fServerSocket = std::make_unique<UnixSocketServer>(Directory::sockPath());
}

/******************************************************************************/
void Session::repaintThread()
{

  //while(fExecuting)
  {
    /* Check if the scene should be repainted. */



  }

}



/******************************************************************************/
void Session::sendFBConfig(Window * window)
{
  /* Iterate through the list for frame buffers. */
  for(auto & fb : fFrameBuffers)
  {
    /* Setup the event. */
    Event::buildFBConfigEvent(fEvent, fb.id(), fb.rect());

    /* Send the event. */
    window->sendEvent(fEvent);
  }
}

/******************************************************************************/
void Session::update()
{
  /* Process all the new window connections. */
  while(auto client = fServerSocket->hasNewConn())
  {
    /* Create the window. */
    auto window = std::make_unique<Window>(client.value());

    /* Send the frame buffer configuratiosn to the window. */
    sendFBConfig(window.get());

    /* Add the window to the list of windows. */
    fWindows.insert(fWindows.begin(), std::move(window));
  }
}

/******************************************************************************/
void Session::repaint()
{

}

/******************************************************************************/
void Session::saveToPNG(const std::string & fileName)
{
  /* Check if there are any frame buffers worth screenshotting. */
  if(fFrameBuffers.size() == 0)
    return;

  /* Calculate the final image size. */
  Rect rect = fFrameBuffers.front().rect();

  for(auto & fb : fFrameBuffers)
    rect = rect.calcUnion(fb.rect());

  /* Create the image. */
  png::image<png::rgba_pixel> image(static_cast<png::uint_32>(rect.width()),
                                    static_cast<png::uint_32>(rect.height()));

  /* Copy the contents of the frame buffers. */
  for(auto & fb : fFrameBuffers)
  {
    /* Calculate the X and Y offset for frame buffer into the dst image. */
    size_t offX = static_cast<size_t>(fb.rect().x() - rect.x());
    size_t offY = static_cast<size_t>(fb.rect().y() - rect.y());

    /* Iterate through all the pixels in the frame buffer and copy them into
     * the PNG. */
    for(size_t y = 0; y < static_cast<size_t>(fb.rect().height()); y++)
    {
      for(size_t x = 0; x < static_cast<size_t>(fb.rect().width()); x++)
      {
        /* Get the current pixel from the frame buffer. */
        Colour pixel = fb.pixels()[y * static_cast<size_t>(rect.width()) + x];

        /* Convert the pixel from BGRA to RGBA. */
        image[y + offY][x + offX] = png::rgba_pixel(pixel.red(),
                                                    pixel.green(),
                                                    pixel.blue(),
                                                    pixel.alpha());
      }
    }
  }

  /* Write the image now. */
  image.write(fileName);
}
