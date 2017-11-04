#include "../../../../Include/Luna/Common.hpp"

using namespace Luna::Common;

Log Log::fInstance;

//==============================================================================
Log::Log() : fWriting(true)
{
  // Create the write thread.
  fWriteThread = std::make_unique<std::thread>(&Log::writeThread, this);
}

//==============================================================================
Log::~Log()
{
  // Stop writting.
  if(fWriting)
  {
    // Tell the thread to stop.
    fWriting = false;

    // Wait for the thread to finish.
    fWriteThread->join();
  }
}

//==============================================================================
void Log::writeThread()
{
  while(fWriting)
  {
    // Flush the current log messages.
    flush();

    // Yield the thread.
    std::this_thread::yield();
  }

  // Flush any remaining log message.
  flush();
}

//==============================================================================
void Log::flush()
{
  while(fMessages.size() > 0)
  {
    // Lock the write mutex.
    std::lock_guard<std::mutex> lock(fMessagesMutex);

    // Pop the message of the front and write it out.
    std::cout << fMessages.front() << std::endl;
    fMessages.pop();
  }
}

//==============================================================================
Log * Log::ptr()
{
  // Return the instance to the object.
  return &fInstance;
}

//==============================================================================
void Log::write(std::string message)
{
  // Lock the write mutex.
  std::lock_guard<std::mutex> lock(fMessagesMutex);

  // Push the message onto the queue.
  fMessages.push(message);
}
