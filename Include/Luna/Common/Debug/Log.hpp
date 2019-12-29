#pragma once
#include <atomic>
#include <mutex>
#include <queue>
#include <sstream>
#include <string>
#include <thread>

#define LUNA_LOG_MSG(lvl, msg) \
{\
  std::string file(__FILE__); \
  size_t i = file.rfind('/', file.length());\
  if (i != std::string::npos) file = file.substr(i+1, file.length() - i);\
  std::stringstream ss; ss << lvl << " | " << file << " | " << \
    __FUNCTION__ << " | " << __LINE__ << " | " << msg;\
  Luna::Log::ptr()->write(ss.str());\
}


#define LUNA_LOG_DEBUG(msg) LUNA_LOG_MSG("DEBUG", msg)
#define LUNA_LOG_INFO(msg) LUNA_LOG_MSG("INFO", msg)
#define LUNA_LOG_WARN(msg) LUNA_LOG_MSG("WARN", msg)
#define LUNA_LOG_ERROR(msg) LUNA_LOG_MSG("ERROR", msg)
#define LUNA_LOG_FATAL_ERROR(msg) LUNA_LOG_MSG("FATAL ERROR", msg)
#define LUNA_LOG_EXCEPTION(ex) LUNA_LOG_MSG("EXCEPTION", ex.what())


namespace Luna
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

