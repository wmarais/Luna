#ifndef LUNA_SERVER_INPUT_MOUSE_HPP
#define LUNA_SERVER_INPUT_MOUSE_HPP

#include "../Input.hpp"

namespace Luna
{
  namespace Server
  {
    class Mouse : public Input
    {
      std::string fPath;

    public:

      Mouse(const std::string & path);
      ~Mouse();

      void monitor(std::shared_ptr<SessionManager> sm);
    };
  }
}

#endif // LUNA_SERVER_INPUT_MOUSE_HPP
