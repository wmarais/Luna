//
// DESIGN NOTES
// ------------
// The SessionManager class is responsible for monitoring the Input devices
// because it is responsible for generating all Events including the non Input
// events. If it was not monitoring the Input devices, then there would be
// another section(s) of code also generating Events. For ease of development
// and maintenance it seems like a simpler solution with no currently forseen
// downsides.
//
// Each input run in a unique thread. This is done to try and minimise input lag
// originating for blocking code. A vector is used for the list since it is
//
//



#ifndef LUNA_SERVER_SESSION_MANAGER_HPP
#define LUNA_SERVER_SESSION_MANAGER_HPP

#include "../Common.hpp"

namespace Luna
{
  namespace Server
  {
    class SessionManager
    {

    public:

      void run();

    };
  }
}

#endif // LUNA_SERVER_SESSION_MANAGER_HPP
