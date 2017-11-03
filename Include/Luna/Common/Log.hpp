#ifndef LUNA_COMMON_LOG_HPP
#define LUNA_COMMON_LOG_HPP

#define LUNA_TRACE_FUNCTION()
#define LUNA_LOG_DEBUG(msg) std::cout << msg << std::endl
#define LUNA_LOG_INFO(msg) std::cout << msg << std::endl
#define LUNA_LOG_WARN(msg) std::cout << msg << std::endl
#define LUNA_LOG_ERROR(msg) std::cout << msg << std::endl
#define LUNA_LOG_FATAL_ERROR(msg) std::cout << msg << std::endl
#define LUNA_LOG_EXCEPTION(msg, ex) std::cout << msg << " | " << ex.what() << std::endl

namespace Luna
{
  namespace Common
  {
    class Log
    {

    public:

      static void create()
      {

      }

      static void destroy()
      {

      }

    };
  }
}

#endif // LUNA_COMMON_LOG_HPP

