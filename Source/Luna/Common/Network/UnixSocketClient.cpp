#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "../../../../Include/Luna/Common/Debug/Exception.hpp"
#include "../../../../Include/Luna/Common/Debug/FunctionTracer.hpp"
#include "../../../../Include/Luna/Common/Debug/Log.hpp"
#include "../../../../Include/Luna/Common/Network/UnixSocketClient.hpp"

using namespace Luna;

/******************************************************************************/
UnixSocketClient::UnixSocketClient(int handle) : fHandle(handle),
  fExecuting(true), fReadException(nullptr), fWriteException(nullptr)
{
  /* Trace the function. */
  LUNA_TRACE_FUNCTION();

  /* Check that the supplied handle is valid. */
  if(fHandle == -1)
  {
    LUNA_THROW_RUNTIME_ERROR("Invalid socket handle provided: " << fHandle);
  }

  /* Start the read thread. */
  fReadThread =
      std::make_unique<std::thread>(&UnixSocketClient::readThreadEntry, this);

  /* Start the write thread. */
  fWriteThread =
      std::make_unique<std::thread>(&UnixSocketClient::writeThreadEntry, this);
}

/******************************************************************************/
UnixSocketClient::UnixSocketClient(const std::string & path) : fHandle(-1),
  fExecuting(true), fReadException(nullptr), fWriteException(nullptr)
{
  /* Trace the function call. */
  LUNA_TRACE_FUNCTION();

  /* Create the new unix socket. */
  fHandle = socket(AF_UNIX, SOCK_STREAM, 0);

  /* Check if the socket is valid. */
  if(fHandle == -1)
  {
    LUNA_THROW_RUNTIME_ERROR("Failed to create socket.");
  }

  /* Initialise the address structure. */
  struct sockaddr_un addr;
  memset(&addr, 0, sizeof(struct sockaddr_un));
  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, path.c_str(), sizeof(addr.sun_path) - 1);

  /* Connect the socket and check if an error occured. */
  if(connect(fHandle, reinterpret_cast<struct sockaddr*>(&addr),
          sizeof(struct sockaddr_un)) == -1)
  {
    LUNA_THROW_RUNTIME_ERROR("Failed to connect unix socket:" <<
                             strerror(errno));
  }

  /* Start the read thread. */
  fReadThread =
      std::make_unique<std::thread>(&UnixSocketClient::readThreadEntry, this);

  /* Start the write thread. */
  fWriteThread =
      std::make_unique<std::thread>(&UnixSocketClient::writeThreadEntry, this);
}

/******************************************************************************/
UnixSocketClient::~UnixSocketClient()
{
  /* Trace the function. */
  LUNA_TRACE_FUNCTION();

  /* Stop execution of the socket. */
  fExecuting = false;

  /* Wait for both threads to finish. */
  fReadThread->join();
  fWriteThread->join();

  /* Check if the socket is open. */
  if(fHandle != -1)
  {
    close(fHandle);
  }
}

/******************************************************************************/
void UnixSocketClient::read(uint8_t * buffer, size_t count)
{
  /* Trace the function. */
  LUNA_TRACE_FUNCTION();

  /* Tracks how many bytes has been read. */
  size_t offset = 0;

  /* Keep reading until all the bytes are read or until an error occured. */
  while(offset < count)
  {
    /* Read the remaining bytes. */
    ssize_t result = ::read(fHandle, buffer + offset, count - offset);

    /* Check if a read error occured. */
    if(result < 0)
    {
      LUNA_THROW_RUNTIME_ERROR("Fail to read() from socket: " <<
                               strerror(errno));
    }

    /* Increment the counter of bytes read. */
    offset += static_cast<size_t>(result);
  }
}

/******************************************************************************/
void UnixSocketClient::write(uint8_t * buffer, size_t count)
{
  /* Trace the function. */
  LUNA_TRACE_FUNCTION();

  /* Tracks how many bytes has been written. */
  size_t offset = 0;

  /* Keep writting until all the bytes has been written or an error occured. */
  while(offset < count)
  {
    /* Write the remaining bytes. */
    ssize_t result = ::write(fHandle, buffer + offset, count - offset);

    /* Check if an error occured. */
    if(result < 0)
    {
      LUNA_THROW_RUNTIME_ERROR("Failed to write() to socket: " <<
                               strerror(errno));
    }

    /* Increment the counter of bytes written. */
    offset += static_cast<size_t>(result);
  }
}

/******************************************************************************/
void UnixSocketClient::readThreadEntry()
{
  /* Trace the function. */
  LUNA_TRACE_FUNCTION();

  /* The read buffer to cache so we dont keep reallocating. */
  std::vector<uint8_t> buff;

  try
  {
    /* Keep reading while executing. */
    while(fExecuting)
    {
      /* The lenght of the block. */
      uint32_t count = 0;
      read(reinterpret_cast<uint8_t*>(&count), sizeof(count));

      /* Read the whole data block. */
      buff.resize(count);
      read(buff.data(), static_cast<size_t>(count));

      /* Put the data block in the queue. */
      bool inserted = false;
      while(!inserted)
      {
        std::scoped_lock l(fReadMutex);
        if(auto dst = fReadQueue.push())
        {
          dst.value()->swap(buff);
          inserted = true;
        }
        sleep(0);
      }
    }
  }
  catch (...)
  {
    /* Store the exception. */
    fReadException = std::current_exception();
  }
}


/******************************************************************************/
void UnixSocketClient::writeThreadEntry()
{
  /* Trace the function. */
  LUNA_TRACE_FUNCTION();

  try
  {
    /* Keep writting while executing. */
    while(fExecuting)
    {
      std::scoped_lock l(fWriteMutex);

      /* Get the next block to write. */
      if(auto src = fWriteQueue.pop())
      {

        /* Write the length of the block. */
        uint32_t len = static_cast<uint32_t>(src.value()->size());
        write(reinterpret_cast<uint8_t*>(&len), sizeof(uint32_t));

        /* Write the rest of the block. */
        write(src.value()->data(), src.value()->size());
      }
    }
  }
  catch (...)
  {
    /* Store the exception. */
    fReadException = std::current_exception();
  }
}

/******************************************************************************/
void UnixSocketClient::rethrowExceptions()
{
  /* Trace the function. */
  LUNA_TRACE_FUNCTION();

  /* Rethrow any write exceptions which has occured. */
  {
    std::scoped_lock l(fWriteMutex);

    if(fWriteException)
      std::rethrow_exception(fWriteException);
  }

  /* Rethrow any read exception which has occured. */
  {
    std::scoped_lock l(fReadMutex);

    if(fReadException)
      std::rethrow_exception(fReadException);
  }
}

/******************************************************************************/
bool UnixSocketClient::writeBlock(std::vector<uint8_t> data)
{
  /* Trace the function. */
  LUNA_TRACE_FUNCTION();

  try
  {
    /* Rethrow any exceptions that has been generated in the other threads. */
    rethrowExceptions();

    /* Lock access to the write queue. */
    std::scoped_lock l(fWriteMutex);

    /* Check if there is any space in the queue. */
    if(auto result = fWriteQueue.push())
    {
      /* Get the reference to the buffer. */
      std::vector<uint8_t> * buff = result.value();

      /* Swap the buffer. */
      buff->swap(data);

      /* Return true to indicate it was queued. */
      return true;
    }

    /* Return false to indicate the block was not queued. */
    return false;
  }
  catch (...)
  {
    /* Rethrow, nesting the previous exception. */
    LUNA_THROW_NESTED_RUNTIME_ERROR("UnixSocketClient::writeBlock() failed.");
  }
}

/******************************************************************************/
bool UnixSocketClient::readBlock(std::vector<uint8_t> & output)
{
  /* Trace the function. */
  LUNA_TRACE_FUNCTION();

  try
  {
    /* Rethrow any exceptions that has been generated in the other threads. */
    rethrowExceptions();

    /* Lock access to the read queue. */
    std::scoped_lock l(fReadMutex);

    /* Check if there are any blocks queued. */
    if(auto result = fReadQueue.pop())
    {
      /* Swap the buffers. */
      result.value()->swap(output);

      /* Return true to indicate that it was read. */
      return true;
    }

    /* There is nothing to read. */
    return false;
  }
  catch (...)
  {
    /* Rethrow, nesting the previous exception. */
    LUNA_THROW_NESTED_RUNTIME_ERROR("UnixSocketClient::readBlock() failed.");
  }
}
