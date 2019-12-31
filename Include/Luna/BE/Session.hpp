#pragma once

#include <memory>
#include <vector>

#include "Window.hpp"
#include "../Common/FrameBuffer.hpp"
#include "../Common/Network/UnixSocketServer.hpp"

namespace Luna::BE
{
  class Session
  {
    enum class States
    {
      kLogin
    };


    /* The socket used to listen for new window requests. */
    std::unique_ptr<UnixSocketServer> fServerSocket;

    /** The list of frame buffers. These correspond to the various displays
     * that is configured. */
    std::vector<FrameBuffer> fFrameBuffers;

    /** The list of windows in the session. */
    std::vector<std::unique_ptr<Window>> fWindows;

    /** A cached Event to reduce memory allocations. */
    Event fEvent;

    void sendFBConfig(Window * window);

    void updateFBConfig();

    void repaintThread();



  public:
    Session();

    void update();
    void redraw();

    /***********************************************************************//**
     * @brief repaint
     **************************************************************************/
    void repaint();

    void saveToPNG(const std::string & path);
  };
}
