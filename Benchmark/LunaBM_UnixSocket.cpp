/*******************************************************************************
 * Micro benchmark for testing the performance of the UnixSocket read and write
 * performance.
 ******************************************************************************/

#include <error.h>
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <syslog.h>
#include <unistd.h>

#include <benchmark/benchmark.h>

#include "../Include/Luna/Common/Debug/Log.hpp"
#include "../Include/Luna/Common/Network/UnixSocketClient.hpp"
#include "../Include/Luna/Common/Network/UnixSocketServer.hpp"

#define LUNA_BM_UNIX_SOCKET_PATH "/tmp/LunaBM_UnixSocket"
#define LUNA_BM_MAX_BLOCK_SIZE 1000000
#define LUNA_BM_NUM_ITERATIONS 1000

/* The PID of the child process. */
pid_t gChildPID;

/* The Socket that is used for communication. */
std::unique_ptr<Luna::UnixSocketClient> fSocket;

void Comparison() {
  /* Create the buffer. */
  std::vector<uint8_t> buffer;
  buffer.resize(1);
  buffer[0] = 0;

  auto start = std::chrono::steady_clock::now();

  for (size_t i = 0; i < LUNA_BM_NUM_ITERATIONS; i++) {
    /* Write the block of data. */
    fSocket->writeBlock(buffer);

    /* Read the block of data. */
    while (!fSocket->readBlock(buffer))
      sleep(0);
  }

  auto end = std::chrono::steady_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << "Call Time: " << (duration.count() / LUNA_BM_NUM_ITERATIONS)
            << std::endl;
}

/******************************************************************************/
void gLunaBM_UnixSocket_ServerToClient_1B(benchmark::State &state) {
  /* Create the buffer. */
  std::vector<uint8_t> buffer;
  buffer.resize(1);
  buffer[0] = 0;

  for (auto _ : state) {
    /* Write the block of data. */
    fSocket->writeBlock(buffer);

    /* Read the block of data. */
    while (!fSocket->readBlock(buffer))
      sleep(0);
  }
}

BENCHMARK(gLunaBM_UnixSocket_ServerToClient_1B)
    ->Iterations(LUNA_BM_NUM_ITERATIONS);

void gLunaBM_UnixSocket_ServerToClient_10B(benchmark::State &state) {
  /* Create the buffer. */
  std::vector<uint8_t> buffer;
  buffer.resize(10);
  buffer[0] = 0;

  for (auto _ : state) {
    /* Write the block of data. */
    fSocket->writeBlock(buffer);

    /* Read the block of data. */
    while (!fSocket->readBlock(buffer))
      sleep(0);
  }
}
BENCHMARK(gLunaBM_UnixSocket_ServerToClient_10B)
    ->Iterations(LUNA_BM_NUM_ITERATIONS);

void gLunaBM_UnixSocket_ServerToClient_100B(benchmark::State &state) {
  /* Create the buffer. */
  std::vector<uint8_t> buffer;
  buffer.resize(100);
  buffer[0] = 0;

  for (auto _ : state) {
    /* Write the block of data. */
    fSocket->writeBlock(buffer);

    /* Read the block of data. */
    while (!fSocket->readBlock(buffer))
      sleep(0);
  }
}
BENCHMARK(gLunaBM_UnixSocket_ServerToClient_100B)
    ->Iterations(LUNA_BM_NUM_ITERATIONS);

void gLunaBM_UnixSocket_ServerToClient_1000B(benchmark::State &state) {
  /* Create the buffer. */
  std::vector<uint8_t> buffer;
  buffer.resize(1000);
  buffer[0] = 0;

  for (auto _ : state) {
    /* Write the block of data. */
    fSocket->writeBlock(buffer);

    /* Read the block of data. */
    while (!fSocket->readBlock(buffer))
      sleep(0);
  }
}
BENCHMARK(gLunaBM_UnixSocket_ServerToClient_1000B)
    ->Iterations(LUNA_BM_NUM_ITERATIONS);

// void gLunaBM_UnixSocket_ServerToClient_10000B(benchmark::State & state)
//{
//  /* Create the buffer. */
//  std::vector<uint8_t> buffer;
//  buffer.resize(10000);
//  buffer[0] = 0;

//  for(auto _ : state)
//  {
//    /* Write the block of data. */
//    fSocket->writeBlock(buffer);

//    /* Read the block of data. */
//    while(!fSocket->readBlock(buffer));
//  }
//}
// BENCHMARK(gLunaBM_UnixSocket_ServerToClient_10000B)->Iterations(LUNA_BM_NUM_ITERATIONS);

// void gLunaBM_UnixSocket_ServerToClient_100000B(benchmark::State & state)
//{
//  /* Create the buffer. */
//  std::vector<uint8_t> buffer;
//  buffer.resize(100000);
//  buffer[0] = 0;

//  for(auto _ : state)
//  {
//    /* Write the block of data. */
//    fSocket->writeBlock(buffer);

//    /* Read the block of data. */
//    while(!fSocket->readBlock(buffer));
//  }
//}
// BENCHMARK(gLunaBM_UnixSocket_ServerToClient_100000B)->Iterations(LUNA_BM_NUM_ITERATIONS);

// void gLunaBM_UnixSocket_ServerToClient_1000000B(benchmark::State & state)
//{
//  /* Create the buffer. */
//  std::vector<uint8_t> buffer;
//  buffer.resize(1000000);
//  buffer[0] = 0;

//  for(auto _ : state)
//  {
//    /* Write the block of data. */
//    fSocket->writeBlock(buffer);

//    /* Read the block of data. */
//    while(!fSocket->readBlock(buffer));
//  }
//}
// BENCHMARK(gLunaBM_UnixSocket_ServerToClient_1000000B)->Iterations(LUNA_BM_NUM_ITERATIONS);

void gLunaBM_UnixSocket_ServerToChild(int argc, char **argv) {
  LUNA_LOG_INFO("Creating a new instance.");

  /* Create the child process. */
  gChildPID = fork();

  /* Check the fork was sucessful. */
  if (gChildPID < 0) {
    std::cerr << "Failed to fork process, error code: " << gChildPID << ", "
              << strerror(errno) << std::endl;
    exit(EXIT_FAILURE);
  }

  /* Check if it's child process. */
  if (gChildPID == 0) {
    /* Open Syslog. */
    setlogmask(LOG_UPTO(LOG_NOTICE));
    openlog("LunaBM_UnixSocket_Client_Proc", LOG_CONS | LOG_PID | LOG_NDELAY,
            LOG_LOCAL1);
    syslog(LOG_NOTICE, "Client running");

    /* Delay for a second so that the server will come up. There is probably a
     * better way of doing this? */
    sleep(1);

    syslog(LOG_NOTICE, "Attempting to conenct.");

    /* Create the client socket. */
    fSocket =
        std::make_unique<Luna::UnixSocketClient>(LUNA_BM_UNIX_SOCKET_PATH);

    /* The temporary buffer. */
    std::vector<uint8_t> buffer;
    buffer.resize(LUNA_BM_MAX_BLOCK_SIZE);

    syslog(LOG_NOTICE, "Connected and waiting to echo data.");

    /* Keep looping until an exit command is send. */
    while (1) {
      if (fSocket->readBlock(buffer)) {
        /* Check if it's a quit command. */
        if (buffer[0] == 1) {
          syslog(LOG_NOTICE, "Client done.");
          closelog();
          exit(EXIT_SUCCESS);
        }

        /* Otherwise, echo the data back. */
        fSocket->writeBlock(buffer);
      }
      sleep(0);
    }
  }
  /* Else it is the server process. */
  else {
    LUNA_LOG_INFO("Server instance running.");

    /* Create the server socket. */
    Luna::UnixSocketServer server(LUNA_BM_UNIX_SOCKET_PATH);

    /* Wait for a client connection. */
    auto conn = server.hasNewConn();

    while (!conn) {
      sleep(0);
      conn = server.hasNewConn();
    }

    LUNA_LOG_INFO("Connection established");

    /* Get the connection. */
    fSocket = std::move(conn.value());

    LUNA_LOG_INFO("Initialising Benchmark");
    ::benchmark::Initialize(&argc, argv);
    // if (::benchmark::ReportUnrecognizedArguments(argc, argv)) return 1;

    LUNA_LOG_INFO("Running benchmark");
    ::benchmark::RunSpecifiedBenchmarks();
    LUNA_LOG_INFO("Finished running benchmark");

    Comparison();

    std::vector<uint8_t> data;
    data.resize(1);
    data[0] = 1;
    fSocket->writeBlock(data);

    exit(EXIT_SUCCESS);
  }
}

int main(int argc, char **argv) {
  unlink(LUNA_BM_UNIX_SOCKET_PATH);
  gLunaBM_UnixSocket_ServerToChild(argc, argv);
}
