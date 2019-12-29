#pragma once

#include "InputDevice.hpp"

namespace Luna::BE
{
  class Joystick : public InputDevice
  {
  public:
    Joystick(std::string path);
    ~Joystick();

    void monitor(std::shared_ptr<SessionManager> sm);
  };
}

