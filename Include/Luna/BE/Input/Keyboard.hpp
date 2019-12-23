#ifndef LUNA_SERVER_INPUT_KEYBOARD_HPP
#define LUNA_SERVER_INPUT_KEYBOARD_HPP

#include "InputDevice.hpp"

namespace Luna
{
  namespace Server
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
}


#endif // LUNA_SERVER_INPUT_KEYBOARD_HPP
