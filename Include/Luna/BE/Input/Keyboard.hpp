#pragma once

#include "InputDevice.hpp"

namespace Luna::BE
{
  class Keyboard : public InputDevice
  {
    std::string fPath;

  public:

    Keyboard(const std::string & path);
    ~Keyboard();

    void monitor(std::shared_ptr<SessionManager> sm);

  };
}
