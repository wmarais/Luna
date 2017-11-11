#ifndef LUNA_COMMON_LOG_HPP
#define LUNA_COMMON_LOG_HPP

#define LUNA_LOG_MSG(lvl, msg) \
{\
  std::string file(__FILE__); \
  size_t I = file.rfind('/', file.length());\
  if (I != std::string::npos) file = file.substr(I+1, file.length() - I);\
  std::stringstream SS; SS << lvl << " | " << file << " | " << \
    __FUNCTION__ << " | " << __LINE__ << " | " << msg;\
  Luna::Common::Log::ptr()->write(SS.str());\
}

#ifdef LUNA_DEBUG
#define LUNA_LOG_DEBUG(msg) LUNA_LOG_MSG("DEBUG", msg)
#else
#define LUNA_LOG_DEBUG
#endif

#define LUNA_LOG_INFO(msg) LUNA_LOG_MSG("INFO", msg)
#define LUNA_LOG_WARN(msg) LUNA_LOG_MSG("WARN", msg)
#define LUNA_LOG_ERROR(msg) LUNA_LOG_MSG("ERROR", msg)
#define LUNA_LOG_FATAL_ERROR(msg) LUNA_LOG_MSG("FATAL ERROR", msg)
#define LUNA_LOG_EXCEPTION(ex) LUNA_LOG_MSG("EXCEPTION", ex.what())


namespace Luna
{
  namespace Common
  {
    class Log
    {
      static Log fInstance;

      std::queue<std::string> fMessages;

      std::unique_ptr<std::thread> fWriteThread;

      std::mutex fMessagesMutex;

      std::atomic_bool fWriting;

      Log();
      ~Log();

      void writeThread();
      void flush();

    public:

      static Log * ptr();
      void write(std::string message);
    };
  }
}

#endif // LUNA_COMMON_LOG_HPP

