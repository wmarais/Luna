#include <vector>
#include <iostream>
#include <unistd.h>
#include "../../Include/Luna/Common.hpp"

using namespace Luna;

#define kLUNA_TEST_UNIX_SOCKET    "/tmp/luna_test"


#define kLUNA_100_BYTES     100
#define kLUNA_1K_BYTES      1000
#define kLUNA_10K_BYTES     10000
#define kLUNA_100K_BYTES    100000
#define kLUNA_1M_BYTES      1000000
#define kLUNA_10M_BYTES     10000000
#define kLUNA_100M_BYTES    100000000

#define kLUNA_TEST_MEM_SIZE 1000000000

enum Tests
{
  kSpeedTest_100Bytes = 0,
  kSpeedTesk_1kBytes,
  kSpeedTest_10kBytes,
  kSpeedTest_100kBytes,
  kSpeedTest_1MBytes,
  kSpeedTest_10MBytes,
  kSpeedTest_100MBytes,
  kSpeedTest_Count
};

static const size_t kSpeedTestSizes[] =
{
  kLUNA_100_BYTES,
  kLUNA_1K_BYTES,
  kLUNA_10K_BYTES,
  kLUNA_100K_BYTES,
  kLUNA_1M_BYTES,
  kLUNA_10M_BYTES,
  kLUNA_100M_BYTES
};

static const size_t kNumTestCycles[] =
{
  kLUNA_TEST_MEM_SIZE/kLUNA_100_BYTES,
  kLUNA_TEST_MEM_SIZE/kLUNA_1K_BYTES,
  kLUNA_TEST_MEM_SIZE/kLUNA_10K_BYTES,
  kLUNA_TEST_MEM_SIZE/kLUNA_100K_BYTES,
  kLUNA_TEST_MEM_SIZE/kLUNA_1M_BYTES,
  kLUNA_TEST_MEM_SIZE/kLUNA_10M_BYTES,
  kLUNA_TEST_MEM_SIZE/kLUNA_100M_BYTES
};

#define kLUNA_NUM_SPEED_TEST_CYCLES   5


/******************************************************************************/
void gLuna_SpeedTest()
{
  std::cout << "TEST - Luna::UnixSocket[Client,Server]" << std::endl;

  /* Iterate through the list of tests. */
  for(auto test = static_cast<size_t>(kSpeedTest_100Bytes);
      test < static_cast<size_t>(kSpeedTest_Count); test++)
  {
    /* Print the test configuration. */
    std::cout << "Test " << test << ": " << kSpeedTestSizes[test] << "[Bytes]"
              << std::endl;

    /* Allocate the memory to be transmitted. */
    std::vector<uint8_t> data(kSpeedTestSizes[test]);

    /* Run the number of transmission. */





  }



}

int main(int argc, char * argv[])
{
  /* Hide the unsused parameters. */
  LUNA_UNUSED_PARAM(argc);
  LUNA_UNUSED_PARAM(argv);

  /* Temporary read buffer. */
  std::vector<uint8_t> readBuffer;

  /* Create a unix socket. */
  UnixSocketServer server(kLUNA_TEST_UNIX_SOCKET);

  /* The list of connected clients. */
  std::vector<std::unique_ptr<UnixSocketClient>> clients;

  /* Indicate whether the quit command has been send. */
  bool executing = true;
  while(executing)
  {
    /* Check if there are any new clients. */
     while(auto result = server.hasNewConn())
     {
       clients.push_back(std::move(result.value()));
     }


    for(auto & client : clients)
    {
      if(client->readBlock(readBuffer))
      {
        std::cout << "Read: " << readBuffer.data() << std::endl;
      }
    }
//    /* Iterate through all the clients. */
//    for(auto iter = clients.begin(); iter != clients.end(); iter++)
//    {
//      /* Process the command from the client if any. */
//      if((*iter)->readBlock(readBuffer))
//      {
//        /* Make sure there is a 0 terminator at the end of the buffer. */
//        readBuffer.push_back(0);
//        std::string str(reinterpret_cast<char*>(readBuffer.data()));

//        /* Print the output to the console. */
//        std::cout << str << std::endl;

//        /* Check if the user wanted to quit. */
//        if(str == "quit")
//        {
//          clients.erase(iter);

//          if(clients.size() == 0)
//            executing = false;
//        }
//      }
//    }
    sleep(0);
  }

  return EXIT_SUCCESS;
}
