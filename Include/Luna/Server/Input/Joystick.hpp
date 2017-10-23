#ifndef LUNA_SERVER_INPUT_CONTROLLER_HPP
#define LUNA_SERVER_INPUT_CONTROLLER_HPP

#include "../Input.hpp"

namespace Luna
{
  namespace Server
  {
    class Joystick : public Input
    {
    public:
      Joystick(std::string path);
      ~Joystick();

      void monitor(std::shared_ptr<SessionManager> sm);
    };
  }
}

#endif // LUNA_SERVER_INPUT_CONTROLLER_HPP
