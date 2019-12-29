#pragma once

#include <memory>

namespace Luna
{
  class UnixSocketClient;
}


namespace Luna::BE
{
  class Session
  {
    enum class States
    {
      kLogin
    };

    /** The Unix Socket that is used to listen for new Window Creations. */
    std::unique_ptr<UnixSocketClient> fSocket;


  public:

  };
}
