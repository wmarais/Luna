#ifndef LUNA_COMMON_EXCEPTION_HPP
#define LUNA_COMMON_EXCEPTION_HPP

#include "GeneralHeaders.hpp"

#define LUNA_THROW_EXCEPTION(msg) \
{\
  std::stringstream ss;\
  ss << __FILE__ << " | " << __FUNCTION__ << "|" << __LINE__ << " | " << msg; \
  throw std::exception(ss.str().c_str());\
}


#define LUNA_THROW_RUNTIME_ERROR(msg) \
{\
  std::stringstream ss;\
  ss << __FILE__ << " | " << __FUNCTION__ << "|" << __LINE__ << " | " << msg; \
  throw std::runtime_error(ss.str().c_str());\
}

#endif // LUNA_COMMON_EXCEPTION_HPP
