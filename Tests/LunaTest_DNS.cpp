#include "../Include/Luna/Common/Debug/Log.hpp"
#include "../Include/Luna/Common/Network/DNS.hpp"
#include <iostream>

using namespace Luna;

#define TEST_HOST_NAME "www.google.com"

int main(int argc, char *argv[])
{
  try
  {
    /* Resolve all the IPv4 Addresses for the host. */
    LUNA_LOG_DEBUG("Resolving IPv4 addresses for: " << TEST_HOST_NAME);
    auto ipv4Addrs = DNS::resolve(TEST_HOST_NAME, IPAddress::Versions::V4);

    /* Resolve all the IPv6 Addressess for host. */
    LUNA_LOG_DEBUG("Resolving IPv6 addresses for: " << TEST_HOST_NAME);
    auto ipv6Addrs = DNS::resolve(TEST_HOST_NAME, IPAddress::Versions::V6);

    /* Print all the addresses. */
    LUNA_LOG_DEBUG("Printing IPv4 Addresses");
    std::cout << "IPv4 Addresses for " << TEST_HOST_NAME << ":" << std::endl;
    for(auto addr : ipv4Addrs)
    {
      std::cout << addr << std::endl;
    }

    /* Print all the V6 addresses. */
    LUNA_LOG_DEBUG("Printing IPv6 Addresses");
    std::cout << std::endl
              << "IPv6 Addresses for " << TEST_HOST_NAME << ":" << std::endl;
    for(auto addr : ipv6Addrs)
    {
      std::cout << addr << std::endl;
    }

    LUNA_LOG_DEBUG("Sucessfully completed.");
  }
  catch(std::exception &ex)
  {
    LUNA_LOG_EXCEPTION(ex);
  }

  return EXIT_SUCCESS;
}
