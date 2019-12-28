#include <vector>
#include "../../Include/Luna/Common.hpp"

using namespace Luna;

#define kLUNA_TEST_UNIX_SOCKET    "/tmp/luna_test"

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
