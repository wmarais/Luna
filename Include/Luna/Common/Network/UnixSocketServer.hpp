#pragma once

#include <atomic>
#include <thread>
#include <mutex>
#include <vector>
#include <memory>
#include <optional>
#include <string>

#include "UnixSocketClient.hpp"

namespace Luna
{
  class UnixSocketServer
  {

    std::atomic_bool fExecuting;

    /* The number of conenction requests to queue. */
    const size_t kBacklog;

    /* The path of the socket. */
    const std::string klPath;

    /* The handle to the socket. */
    int fHandle;

    /* The thread that is used for listening and accepting new connection
     * requests. */
    std::unique_ptr<std::thread> fThread;

    /* Mutex used to control access to the fnewConnSocket vector. */
    std::mutex fNewConnMutex;

    /* The list of new connections. */
    std::vector<std::unique_ptr<UnixSocketClient>> fNewConnSockets;

    /* The entry functon for the thread. */
    void listenThreadEntry();

  public:

    /***********************************************************************//**
     * Create a new server socket and start listening for connections. This is
     * asynchronous and will not block execution of the program.
     *
     * @param path    The name of the unix socket.
     * @param backlog The maximum number of concurrent connections to queue.
     **************************************************************************/
    UnixSocketServer(const std::string & path, size_t backlog = 256);

    ~UnixSocketServer();


    std::optional<std::unique_ptr<UnixSocketClient>> hasNewConn();
  };
}
