#pragma once

#include <atomic>
#include <thread>
#include <mutex>
#include <vector>
#include <memory>
#include <optional>
#include <string>

namespace Luna
{
  class UnixSocketClient;

  class UnixSocketServer
  {
    /** The states of the listening thread. */
    enum States
    {
      kRunning,
      kStopped,
      kException
    };

    /** The current state of the listening thread. */
    std::atomic<States> fState;

    /** The number of conenction requests to queue. */
    const size_t kBacklog;

    /* The handle to the socket. */
    int fHandle;

    /** The exception that was thrown in the listening thread. */
    std::exception_ptr fListenException;

    /** The thread that is used for listening and accepting new connection
     * requests. */
    std::unique_ptr<std::thread> fThread;

    /** Mutex used to control access to the fnewConnSocket vector. */
    std::mutex fNewConnMutex;

    /** The list of new connections. */
    std::vector<std::unique_ptr<UnixSocketClient>> fNewConnSockets;

    /** The entry functon for the thread. */
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

    /***********************************************************************//**
     * Terminate the listening thread and close the socket.
     **************************************************************************/
    ~UnixSocketServer();

    /***********************************************************************//**
     * Check if any exceptions has been thrown in the listening thread and
     * rethrow it. This function must only be called from the main thread. This
     **************************************************************************/
    void rethrowException();

    /***********************************************************************//**
     * Check if a new connection has been established and return it if it has.
     *
     * @return  The new connection if one has been established or false.
     **************************************************************************/
    std::optional<std::unique_ptr<UnixSocketClient>> hasNewConn();
  };
}
