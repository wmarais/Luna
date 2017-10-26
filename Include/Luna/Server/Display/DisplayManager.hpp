#ifndef LUNA_SERVER_DISPLAY_DISPLAY_MANAGER_HPP
#define LUNA_SERVER_DISPLAY_DISPLAY_MANAGER_HPP

#include "../UDEV.hpp"

namespace Luna
{
  namespace Server
  {
    class DisplayManager
    {

    public:

      void manageDevice(const std::string & devNode,
                        UDEV::DeviceActions action);
    };
  }
}


#endif // LUNA_SERVER_DISPLAY_DISPLAY_MANAGER_HPP
