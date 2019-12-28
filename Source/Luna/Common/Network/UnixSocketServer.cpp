#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "../../../../Include/Luna/Common/Debug/Exception.hpp"
#include "../../../../Include/Luna/Common/Debug/FunctionTracer.hpp"
#include "../../../../Include/Luna/Common/Debug/Log.hpp"
#include "../../../../Include/Luna/Common/Network/UnixSocketServer.hpp"

using namespace Luna;

/******************************************************************************/
UnixSocketServer::UnixSocketServer(const std::string & path, size_t backlog) :
  fExecuting(true), kBacklog(backlog), klPath(path), fHandle(-1)
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

  unlink(path.c_str());

  /* Initialise the address structure. */
  struct sockaddr_un addr;
  memset(&addr, 0, sizeof(struct sockaddr_un));
  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, klPath.c_str(), sizeof(addr.sun_path) - 1);

  /* Bind the socket and check if an error occured. */
  if(bind(fHandle, reinterpret_cast<struct sockaddr*>(&addr),
          sizeof(struct sockaddr_un)) == -1)
  {
    LUNA_THROW_RUNTIME_ERROR("Failed to bind unix socket.");
  }

  /* Create the thread used for listening for new conenctions. */
  fThread = std::make_unique<std::thread>(&UnixSocketServer::listenThreadEntry,
                                          this);
}

/******************************************************************************/
UnixSocketServer::~UnixSocketServer()
{
  /* Trace the function call. */
  LUNA_TRACE_FUNCTION();

  /* Stop the listening thread. */
  fExecuting = false;

  /* Wait for the thread to finish. */
  if(fThread)
  {
    fThread->join();
  }

  /* Close the socket. */
  if(fHandle != -1)
  {
    close(fHandle);
  }
}

/******************************************************************************/
void UnixSocketServer::listenThreadEntry()
{
  /* Trace the function call. */
  LUNA_TRACE_FUNCTION();

  /* Keep looping while waiting for new network connections. */
  while(fExecuting)
  {
    /* Listen for new connection requests. */
    if(listen(fHandle, static_cast<int>(kBacklog)) == -1)
    {
      LUNA_THROW_RUNTIME_ERROR("Unix Socket listen() for connections: " <<
                               strerror(errno));
    }

    /* Accept the new connection. */
    int handle = accept(fHandle, nullptr, nullptr);

    /* Check if an error occured. */
    if(handle == -1)
    {
      LUNA_THROW_RUNTIME_ERROR("Unix Socket failed to accept() connection: " <<
                               strerror(errno));
    }

    /* Create the new socket. */
    std::unique_ptr<UnixSocketClient> socket =
        std::make_unique<UnixSocketClient>(handle);

    /* Add it to the queue. */
    {
      std::scoped_lock l(fNewConnMutex);
      fNewConnSockets.push_back(std::move(socket));
    }
  }
}

/******************************************************************************/
std::optional<std::unique_ptr<UnixSocketClient>> UnixSocketServer::hasNewConn()
{
  /* Lock access to the list. */
  std::scoped_lock l(fNewConnMutex);

  /* Check if there are any new connections. */
  if(fNewConnSockets.size() == 0)
    return {};

  /* Return the first available client connection. */
  auto result = std::move(fNewConnSockets.front());
  fNewConnSockets.erase(fNewConnSockets.begin());
  return std::move(result);
}
