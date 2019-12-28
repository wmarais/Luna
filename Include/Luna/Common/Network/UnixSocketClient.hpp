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
  /*************************************************************************//**
   * Socket interface used for communicating between two end points. The
   * interface is designed that the vector<> supplied to writeBlock() and
   * writeBlockSwap() specify the boundary of a data block. That is the use
   * will read the exact block of data as was written on the remote socket and
   * visa versa.
   *
   * This class was specifically designd for fast inter process communication
   * (IPC) between the Front End (FE) and the Back End (BE) in order to reduce
   * total IO latencies.
   ****************************************************************************/
  class UnixSocketClient final
  {
    /** The maximum number of blocks to queue. */
    static const size_t kMaxIOQueueSize = 256;

    /** The handle of the socket. */
    int fHandle;

    /** Indicate whether the socket is running. */
    std::atomic_bool fExecuting;

    /** Is set when a read exception occurs in the Read Thread. */
    std::exception_ptr fReadException;

    /** Is set when a write exception occurs in the Write Thread. */
    std::exception_ptr fWriteException;

    /** The thread used for reading blocks. */
    std::unique_ptr<std::thread> fReadThread;

    /** The thread used for writting blocks. */
    std::unique_ptr<std::thread> fWriteThread;

    /** The mutex used to control access to the Read Queue. */
    std::mutex fReadMutex;

    /** The mutex used to control access to the Write Queue. */
    std::mutex fWriteMutex;

    /** The Circular Queue caching the blocks that's been read. */
    CircularQueue<std::vector<uint8_t>, kMaxIOQueueSize> fReadQueue;

    /** The Circular Queue caching the blocks to be written. */
    CircularQueue<std::vector<uint8_t>, kMaxIOQueueSize> fWriteQueue;

    /** Delete the default constructor. */
    UnixSocketClient() = delete;

    /** Delete the copy constructor. */
    UnixSocketClient(const UnixSocketClient&) = delete;

    /** Delete the assignment operator. */
    UnixSocketClient & operator = (const UnixSocketClient&) = delete;

    /***********************************************************************//**
     * Read the specified amount of bytes from the socket and block until it
     * is done. It is expected that 'buffer' will be adquately large to store
     * 'count' number of bytes.
     *
     * @param buffer  The destination buffer where the data will be written too.
     * @param count   The number of bytes to read.
     **************************************************************************/
    void read(uint8_t * buffer, size_t count);

    /***********************************************************************//**
     * Write the specified amount of bytes to the socket and block until it is
     * done.
     *
     * @param buffer  The data to write.
     * @param count   The number of bytes to write.
     **************************************************************************/
    void write(const uint8_t * const buffer, size_t count);

    /***********************************************************************//**
     * The entry function for the thread that continuously read from the socket.
     **************************************************************************/
    void readThreadEntry();

    /***********************************************************************//**
     * The entry function for the thread that continuously write to the socket.
     **************************************************************************/
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
     * Queue a block of data to be written to the socket. this function will
     * make a copy of the data before transmitting it. Since memory copies
     * are slow, if the data will not be use again after this functions is
     * called, then use the writeBlockSwap() instead.
     *
     * @param data  The data block to be written.
     * @return      True if the block was queued or false if not. False should
     *              only ever be returned if the queue is full.
     **************************************************************************/
    bool writeBlock(std::vector<uint8_t> data);

    /***********************************************************************//**
     * Does the exact same as writeBlock() except for it does not copy the data
     * it simply swaps the buffers. Though the data vector will still be a valid
     * vector object after the swap, the data inside will be corrupt as
     * literally anything could have been swapped in.
     *
     * @param data  The data block to be written.
     * @return      True if the block was queued or false if not. False should
     *              only ever be returned if the queue is full.
     **************************************************************************/
    bool writeBlockSwap(std::vector<uint8_t> & data);

    /***********************************************************************//**
     * Read a block of data from the Unix Socket.
     *
     * @param output  The destination to which the data block will be read.
     * @return
     **************************************************************************/
    bool readBlock(std::vector<uint8_t> & output);
  };
}

