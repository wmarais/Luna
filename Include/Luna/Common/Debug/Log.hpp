#pragma once
#include "../../Config.hpp"
#include "../CircularQueue.hpp"
#include <atomic>
#include <mutex>
#include <queue>
#include <sstream>
#include <string>
#include <thread>
#include <tuple>

/******************************************************************************/
#define LUNA_LOG_MSG(lvl, msg)                                                 \
  {                                                                            \
    std::string _luna_file(__FILE__);                                          \
    size_t _luna_i = _luna_file.rfind('/', _luna_file.length());               \
    if(_luna_i != std::string::npos)                                           \
      _luna_file =                                                             \
          _luna_file.substr(_luna_i + 1, _luna_file.length() - _luna_i);       \
    std::stringstream _luna_ss;                                                \
    _luna_ss << _luna_file << " | " << __FUNCTION__ << " | " << __LINE__       \
             << " | " << msg;                                                  \
    Luna::Log::ptr()->write(lvl, _luna_ss.str());                              \
  }

/******************************************************************************/
#ifdef LUNA_CONF_ENABLE_LOG_EMERGENCY
#define LUNA_LOG_EMERGENCY(msg) LUNA_LOG_MSG(Luna::Log::Levels::Emergency, msg)
#define LUNA_LOG_EXCEPTION(ex)                                                 \
  LUNA_LOG_MSG(Luna::Log::Levels::Emergency, ex.what())
#else
#define LUNA_LOG_EMERGENCY(msg)
#define LUNA_LOG_EXCEPTION(ex)
#endif /* LUNA_CONF_ENABLE_LOG_EMERGENCY */

/******************************************************************************/
#ifdef LUNA_CONF_ENABLE_LOG_ALERT
#define LUNA_LOG_ALERT(msg) LUNA_LOG_MSG(Luna::Log::Levels::Alert, msg)
#else
#define LUNA_LOG_ALERT
#endif /* LUNA_CONF_ENABLE_LOG_ALERT */

/******************************************************************************/
#ifdef LUNA_CONF_ENABLE_LOG_CRITICAL
#define LUNA_LOG_CRITICAL(msg) LUNA_LOG_MSG(Luna::Log::Levels::Critical, msg)
#else
#define LUNA_LOG_CRITICAL
#endif /* LUNA_CONF_ENABLE_LOG_CRITICAL */

/******************************************************************************/
#ifdef LUNA_CONF_ENABLE_LOG_ERROR
#define LUNA_LOG_ERROR(msg) LUNA_LOG_MSG(Luna::Log::Levels::Error, msg)
#else
#define LUNA_LOG_ERROR(msg)
#endif /* LUNA_CONF_ENABLE_LOG_ERROR */

/******************************************************************************/
#ifdef LUNA_CONF_ENABLE_LOG_WARNING
#define LUNA_LOG_WARNING(msg) LUNA_LOG_MSG(Luna::Log::Levels::Warning, msg)
#else
#define LUNA_LOG_WARNING(msg)
#endif /* LUNA_CONF_ENABLE_LOG_WARNING */

/******************************************************************************/
#ifdef LUNA_CONF_ENABLE_LOG_NOTICE
#define LUNA_LOG_NOTICE(msg) LUNA_LOG_MSG(Luna::Log::Levels::Notice, msg)
#else
#define LUNA_LOG_NOTICE(msg)
#endif /* LUNA_CONF_ENABLE_LOG_NOTICE */

/******************************************************************************/
#ifdef LUNA_CONF_ENABLE_LOG_INFO
#define LUNA_LOG_INFO(msg) LUNA_LOG_MSG(Luna::Log::Levels::Info, msg)
#else
#define LUNA_LOG_INFO(msg)
#endif /* LUNA_CONF_ENABLE_LOG_INFO */

#ifdef LUNA_CONF_ENABLE_LOG_DEBUG
#define LUNA_LOG_DEBUG(msg) LUNA_LOG_MSG(Luna::Log::Levels::Debug, msg)
#else
#define LUNA_LOG_DEBUG(msg)
#endif /* LUNA_CONF_ENABLE_LOG_DEBUG */

/******************************************************************************/
namespace Luna
{
  /**
   * A C++ wrapper around Syslog adding the capability to queue log messages
   * prior to being written out. It is expected that the single copy operation
   * between memory and the locing of the mutex is overall faster than
   * sycnhronous log IO.
   */
  class Log final
  {
  public:
    /** The log levels in order of priority, mapping directly to the syslog
     * log levels. */
    enum class Levels : int
    {
      /** System is unusable. This can be due to hardware failures, such as
       * required input devices being missing to software problems such as
       * detection of malicious intrusion and the system acting to protect
       * itself. */
      Emergency = 0,

      /** Action must be taken immediately. */
      Alert,

      /** Critical condictions which will result in the termination of the
       * application. */
      Critical,

      /** Error conditions which does not result in the application dying. For
       * example, failing to open a non critical configuration file, but inbuilt
       * defaults are adequate. */
      Error,

      /** Warning conditions generally warning of unexpected issues that will
       * not affect the operation of the software, but is generally bad. For
       * example a file or directory which has permissions set to loosely, but
       * will likely not result in data loss or damage. */
      Warning,

      /** General nice to know information that can be very useful to a network
       * aministrator to understand the general health and status of the
       * system. */
      Notice,

      /** Verbose General Nice to know information. */
      Info,

      /** Literally 90% of all log messages. */
      Debug
    };

    /**
     * Return the pointer to the singleton instance of the Log object.
     *
     * @return  Singleton instance of the Log object.
     */
    static Log *ptr();

    /**
     * Queue the message for writting to syslog and the console if enabled. This
     * function generally run asynchronously, but will block when the log queue
     * is full. It will also truncate the size of the log message if it exceeds
     * the maximum log message length.
     *
     * @param lvl       The log level of the message.
     * @param message   The message to be logged.
     */
    void write(Levels lvl, std::string msg);

    /**
     * Set the minmumum level of information to be logged. (Emergency is the
     * highest and Debug is the lowest.)
     *
     * @param level The lowest level of log information to be logged.
     */
    void setMinLevel(Levels lvl);

    void setName(const std::string &name);

  private:
    /** The default log level which is generally Debug for when Luna is build
     * in debug mode, and Notice, when Luna is build in any other mode such
     * as for release. */
    static const Levels kDefaultLevel;

    /** The singleton instance of the log. */
    static Log fInstance;

    /** The minimum log level to log up too. */
    Levels fCurLogLevel;
    Levels fNextLogLevel;

    /** The name of the application as will be shown in the log. */
    std::string fName;

    /** The circular queue of log messages. */
    CircularQueue<std::tuple<Levels, std::string>, LUNA_CONF_MAX_LOG_QUEUE_LEN>
        fMsgQueue;

    std::unique_ptr<std::thread> fWriteThread;

    std::mutex fMutex;

    std::atomic_bool fWriting;

    /** Is set when a write exception occurs in the Write Thread. */
    std::exception_ptr fWriteException;

    /** Delete the copy constructory. */
    Log(const Log &) = delete;

    /** Delete the assignment operator. */
    Log &operator=(const Log &) = delete;

    /** The constructor is private so that no external instance of the log can
     * be created. */
    Log();

    /** The destructor is private so that no external delete can be invoked on
     * the log. */
    ~Log();

    /** The thread that is used for writting the log messages out. */
    void writeThread();

    /* Write all the messages currently in the message queue. */
    void flush();
  };
} /* namespace Luna */
