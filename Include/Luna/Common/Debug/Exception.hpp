#ifndef LUNA_COMMON_EXCEPTION_HPP
#define LUNA_COMMON_EXCEPTION_HPP

#include <sstream>

#define LUNA_THROW_EXCEPTION(msg) \
{\
  std::string file(__FILE__); \
  size_t i = file.rfind('/', file.length());\
  if (i != std::string::npos) file = file.substr(i+1, file.length() - i);\
  std::stringstream ss; ss << "Source: " << file << "," << \
    __FUNCTION__ << "," << __LINE__ << " - " << msg;\
  throw std::exception(ss.str().c_str());\
}

#define LUNA_THROW_RUNTIME_ERROR(msg) \
{\
  std::string file(__FILE__); \
  size_t i = file.rfind('/', file.length());\
  if (i != std::string::npos) file = file.substr(i+1, file.length() - i);\
  std::stringstream ss; ss << "Source: " << file << "," << \
    __FUNCTION__ << "," << __LINE__ << " - " << msg;\
  throw std::runtime_error(ss.str().c_str());\
}

#define LUNA_THROW_NESTED_RUNTIME_ERROR(msg) \
{\
  std::string file(__FILE__); \
  size_t i = file.rfind('/', file.length());\
  if (i != std::string::npos) file = file.substr(i+1, file.length() - i);\
  std::stringstream ss; ss << "Source: " << file << "," << \
    __FUNCTION__ << "," << __LINE__ << " - " << msg;\
  std::throw_with_nested(std::runtime_error(ss.str().c_str()));\
}


/*#define LUNA_THROW_EXCEPTION(msg) \
{\
  std::stringstream ss;\
  ss << __FILE__ << " | " << __FUNCTION__ << "|" << __LINE__ << " | " << msg; \
  throw std::exception(ss.str().c_str());\
}*/


/*#define LUNA_THROW_RUNTIME_ERROR(msg) \
{\
  std::stringstream ss;\
  ss << __FILE__ << " | " << __FUNCTION__ << "|" << __LINE__ << " | " << msg; \
  throw std::runtime_error(ss.str().c_str());\
}*/

#endif // LUNA_COMMON_EXCEPTION_HPP
