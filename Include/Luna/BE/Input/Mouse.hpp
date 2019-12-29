#pragma once

#include "InputDevice.hpp"

namespace Luna::BE
{
  class Mouse : public InputDevice
  {
    std::string fPath;

  public:

    Mouse(const std::string & path);
    ~Mouse();

    void monitor(std::shared_ptr<SessionManager> sm);
  };
}
