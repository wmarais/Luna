#pragma once

#include <atomic>
#include <thread>
#include <mutex>
#include <vector>
#include <memory>
#include <optional>
#include <string>

#include "../CircularQueue.hpp"

namespace Luna
{
  class UnixSocketClient
  {

    static const size_t kMaxIOQueueSize = 256;

    /* The handle of the socket. */
    int fHandle;

    /* Indicate whether the socket is running. */
    std::atomic_bool fExecuting;

    std::exception_ptr fReadException;

    std::exception_ptr fWriteException;

    std::unique_ptr<std::thread> fReadThread;
    std::unique_ptr<std::thread> fWriteThread;


    std::mutex fReadMutex;

    std::mutex fWriteMutex;


    CircularQueue<std::vector<uint8_t>, kMaxIOQueueSize> fReadQueue;

    CircularQueue<std::vector<uint8_t>, kMaxIOQueueSize> fWriteQueue;


    void read(uint8_t * buffer, size_t count);
    void write(uint8_t * buffer, size_t count);



    UnixSocketClient() = delete;

    void readThreadEntry();

    void writeThreadEntry();

  public:

    /***********************************************************************//**
     * Create a new socket object using the existing connected client
     * connection. This is used by the UnixSocketServer class when accepting
     * new connection requests.
     *
     * @param handle  The OS handle to the accepted client socket.
     **************************************************************************/
    UnixSocketClient(int handle);

    /***********************************************************************//**
     * Create new client connection to the specified Unix Socket.
     *
     * @param path  The path to the unix socket.
     **************************************************************************/
    UnixSocketClient(const std::string & path);

    /***********************************************************************//**
     * Shut down the Read / Write threads and close the socket cleanly.
     **************************************************************************/
    ~UnixSocketClient();

    /***********************************************************************//**
     * Rethrow any exceptions that occured in either the Read or Write threads
     * into the main thread. This function must be called in main thread for
     * the rethrow to work. It is called during each readBlock() and
     * writeBlock() function call.
     **************************************************************************/
    void rethrowExceptions();

    /***********************************************************************//**
     * Queue a block of data to be written to the socket.
     *
     * @param data  The data block to be written.
     * @return      True if the block was queued or false if not. False should
     *              only ever be returned if the queue is full.
     **************************************************************************/
    bool writeBlock(std::vector<uint8_t> data);

    /***********************************************************************//**
     * Read a block of data from the Unix Socket.
     *
     * @param output  The destination to which the data block will be read.
     * @return
     **************************************************************************/
    bool readBlock(std::vector<uint8_t> & output);
  };
}

