#ifndef LUNA_COMMON_HPP
#define LUNA_COMMON_HPP

#include "Common/GeneralHeaders.hpp"
#include "Common/Event.hpp"
#include "Common/Debug/Exception.hpp"
#include "Common/Debug/Log.hpp"
#include "Common/Debug/FunctionTracer.hpp"
#include "Common/Settings.hpp"


#define LUNA_INLINE inline


namespace Luna
{
  namespace Common
  {
    //==========================================================================
    // An exception occurs if std::string is initialised with a NULL pointer
    // which can happen when dealing with C strings. This function aliviate the
    // the problem by initialising the std::string as an empty string ("") if a
    // a NULL pointer is passed for str.
    //==========================================================================
    LUNA_INLINE static std::string toString(const char * str)
    {
      return std::string(str ? str : "");
    }

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
}


#endif // LUNA_COMMON_HPP
