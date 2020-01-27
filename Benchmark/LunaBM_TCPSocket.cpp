#include <error.h>
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <syslog.h>
#include <unistd.h>

#include <benchmark/benchmark.h>

#include "../Include/Luna/Common/Debug/Log.hpp"

#include "../Include/Luna/Common/Network/DNS.hpp"

using namespace Luna;

/******************************************************************************/
int gLunaBM_TCPSocketClient()
{
  /* Get the IP address from DNS. */
  auto ips = DNS::resolve("localhost", IPAddress::Versions::V4);

  /* Check if any IP addresses was found. */
  if(ips.size() == 0)
  {
  }
}

/******************************************************************************/
int gLunaBM_TCPSocketServer() {}

/******************************************************************************/
int main(int argc, char *argv[])
{
  /* Split the process. */
  pid_t pid = fork();

  /* Check the fork was sucessful. */
  if(pid < 0)
  {
    std::cerr << "Failed to fork process, error code: " << pid << ", "
              << strerror(errno) << std::endl;
    exit(EXIT_FAILURE);
  }

  /* Check if it's the client. */
  if(pid == 0)
    return gLunaBM_TCPSocketClient();

  return gLunaBM_TCPSocketServer();
}
