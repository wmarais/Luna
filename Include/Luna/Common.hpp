#ifndef LUNA_COMMON_HPP
#define LUNA_COMMON_HPP

#include "Common/Debug/Exception.hpp"
#include "Common/Debug/FunctionTracer.hpp"
#include "Common/Debug/Log.hpp"

#include "Common/Network/UnixSocketClient.hpp"
#include "Common/Network/UnixSocketServer.hpp"

#include "Common/CircularQueue.hpp"
#include "Common/Colour.hpp"
#include "Common/Directory.hpp"
#include "Common/Event.hpp"
#include "Common/Macros.hpp"
#include "Common/Rect.hpp"
#include "Common/Settings.hpp"
#include "Common/SharedResource.hpp"
#include "Common/User.hpp"
#include "Common/Utils.hpp"


namespace Luna
{
  template <typename R, typename D> class NonPtrCResource
  {
    R fResource;
    D fDeleter;
    bool fPassTest;
  public:
    NonPtrCResource(const R & resource, bool passTest, const D & deleter) :
      fResource(resource), fDeleter(deleter), fPassTest(passTest) {}

    ~NonPtrCResource()
    {
      fDeleter(fResource);
    }
  };
}


#endif // LUNA_COMMON_HPP
