#ifndef LUNA_COMMON_SHARED_RESOURCE_HPP
#define LUNA_COMMON_SHARED_RESOURCE_HPP

namespace Luna
{
  namespace Common
  {
    template <typename R, typename D, typename ...> class SharedResource
    {
      R fResource;
    public:
      SharedResource(const R & resource,  const D & deleter)
      {

      }

    };
  }
}

#endif // LUNA_COMMON_SHARED_RESOURCE_HPP
