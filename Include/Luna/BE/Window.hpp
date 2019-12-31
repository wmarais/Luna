#pragma once

#include "../Common/Event.hpp"
#include "../Common/Network/UnixSocketClient.hpp"

namespace Luna::BE
{
  class Window
  {
    std::unique_ptr<UnixSocketClient> fSocket;

    /** The temporary buffer cache used to avoid memory allocation. */
    std::vector<uint8_t> fEventBuffer;

  public:
    Window(std::unique_ptr<UnixSocketClient> & socket);

    void sendEvent(const Event & event);

  };
}
