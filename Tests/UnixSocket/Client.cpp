#include <vector>
#include <memory>
#include <iostream>
#include <unistd.h>
#include <cstring>

#include "../../Include/Luna/Common.hpp"

using namespace Luna;

#define kLUNA_TEST_UNIX_SOCKET    "/tmp/luna_test"

int main(int argc, char * argv[])
{
  /* Hide the unsused parameters. */
  LUNA_UNUSED_PARAM(argc);
  LUNA_UNUSED_PARAM(argv);

  /* Create a client socket. */
  UnixSocketClient client(kLUNA_TEST_UNIX_SOCKET);

  bool executing = true;

  std::string command;

  std::vector<uint8_t> buffer;

  while(executing)
  {
    std::cout << ">> ";
    std::cin >> command;

    buffer.resize(command.length());
    memcpy(buffer.data(), command.c_str(), command.length());
    buffer.push_back(0);

    client.writeBlock(buffer);

    sleep(0);
  }

  return EXIT_SUCCESS;
}
