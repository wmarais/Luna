#ifndef LUNA_SERVER_INPUT_CONTROLLER_HPP
#define LUNA_SERVER_INPUT_CONTROLLER_HPP

#include "../Input.hpp"

namespace Luna
{
  namespace Server
  {
    class Controller : public Input
    {
    public:
      Controller(std::string path);
      ~Controller();

      void monitor(std::shared_ptr<SessionManager> sm);
    };
  }
}

#endif // LUNA_SERVER_INPUT_CONTROLLER_HPP
