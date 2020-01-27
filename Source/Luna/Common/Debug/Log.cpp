#include "../../../../Include/Luna/Common/Debug/Log.hpp"
#include "../../../../Include/Luna/Common/SystemHeaders.hpp"
#include <iostream>

using namespace Luna;

/******************************************************************************/
#ifdef LUNA_CONF_DEBUG_BUILD
const Log::Levels Log::kDefaultLevel = Log::Levels::Debug;
#else
const Log::Levels Log::KDefaultLevel = Log::Levels::Notice;
#endif /* LUNA_CONF_DEBUG_BUILD */

Log Log::fInstance;

/******************************************************************************/
Log::Log()
    : fCurLogLevel(kDefaultLevel), fNextLogLevel(kDefaultLevel),
      fWriting(false), fWriteException(nullptr)
{
  /* Create the write thread. */
  fWriteThread = std::make_unique<std::thread>(&Log::writeThread, this);

  /* Wait for the writing thread to be ready. */
  while(!fWriting)
  {
    /* Lock the write mutex. */
    std::lock_guard<std::mutex> lock(fMutex);

    /* Check if a write exception was raised or not. This is protected by the
     * write mutex. */
    if(fWriteException)
      std::rethrow_exception(fWriteException);

    sleep(0);
  }
}

/******************************************************************************/
Log::~Log()
{
  /* Tell the thread to stop. */
  fWriting = false;

  /* Wait for the thread to finish. */
  fWriteThread->join();
}

/******************************************************************************/
void Log::writeThread()
{
  try
  {
    /* Set the starting log mask. */
    setlogmask(LOG_UPTO(static_cast<int>(fCurLogLevel)));

    /* Open the log. */
    openlog(nullptr, LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);

    /* Inidicate that the log is ready to write. */
    fWriting = true;

    while(fWriting)
    {
      /* Flush the current log messages. */
      flush();

      /* Give up the CPU so something else can do something with it. */
      //      sleep(0);
      std::this_thread::yield();
    }

    /* Flush any remaining log message. */
    flush();

    /* Close down syslog. */
    closelog();
  }
  catch(...)
  {
    /* Lock the write mutex. */
    std::lock_guard<std::mutex> lock(fMutex);

    /* Store the exception. */
    fWriteException = std::current_exception();
  }

  /* Mark the thread as not executing anymore. */
  fWriting = false;
}

/******************************************************************************/
void Log::flush()
{
  /* Keep looping until everything is flushed. */
  while(true)
  {
    //    sleep(0);
    {
      /* Lock the write mutex. */
      std::lock_guard<std::mutex> lock(fMutex);

      /* Check if the log levels must change. */
      if(fCurLogLevel != fNextLogLevel)
      {
        /* Change the log levels. */
        setlogmask(LOG_UPTO(static_cast<int>(fNextLogLevel)));
        fCurLogLevel = fNextLogLevel;
      }

      /* Get the next message to write. */
      if(auto msg = fMsgQueue.pop())
      {
        /* Write the message out to syslog. */
        syslog(static_cast<int>(std::get<0>(*msg.value())), "%s",
            std::get<1>(*msg.value()).c_str());
      }
      else
      {
        /* Nothing left to do. */
        return;
      }
    }
  }
}

/******************************************************************************/
Log *Log::ptr()
{
  /* Return the instance to the object. */
  return &fInstance;
}

/******************************************************************************/
void Log::setMinLevel(Levels lvl)
{
  /* Lock the write mutex. */
  std::lock_guard<std::mutex> lock(fMutex);

  /* Record the log level to switch too. */
  fNextLogLevel = lvl;
}
/******************************************************************************/
void Log::write(Levels lvl, std::string msg)
{
  /* Check if the log message is oversize. */
  if(msg.length() > LUNA_CONF_MAX_LOG_MSG_LEN)
  {
    /* Drop the excess message contents. */
    msg.resize(LUNA_CONF_MAX_LOG_MSG_LEN);

    /* Append a notification that the message has been truncated. */
    msg.append("...[MSG TRUNCATED]");
  }

  /** Loop for as long as necesary to write the message to the queue, but only
   * if the writing thread is still running. */
  while(fWriting)
  {
    /* Lock the queue mutex. */
    std::lock_guard<std::mutex> lock(fMutex);

    /* Check if a write exception was raised or not. This is protected by the
     * write mutex. */
    if(fWriteException)
      std::rethrow_exception(fWriteException);

    /* Make sure to rethrow any exceptions from the write thread, else this can
     * infinitely block in the case of an exception. */

    /* Check if the message can be inserted. */
    if(auto item = fMsgQueue.push())
    {
      /* Swap the messages in. */
      std::get<0>(*item.value()) = lvl;
      std::get<1>(*item.value()).swap(msg);

      /* Nothing left to do. */
      return;
    }

    /* Release the thread so we dont needlessly thrash the CPU. */
    //    sleep(0);
    std::this_thread::yield();
  }
}
