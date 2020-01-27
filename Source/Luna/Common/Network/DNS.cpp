#include <algorithm>

#include "../../../../Include/Luna/Common/Debug/Exception.hpp"
#include "../../../../Include/Luna/Common/Debug/FunctionTracer.hpp"
#include "../../../../Include/Luna/Common/Debug/Log.hpp"
#include "../../../../Include/Luna/Common/Network/DNS.hpp"
#include "../../../../Include/Luna/Common/SystemHeaders.hpp"

using namespace Luna;

/******************************************************************************/
std::vector<IPAddress> DNS::resolve(
    const std::string &host, IPAddress::Versions version)
{
  /* Trace the function to the log file. */
  LUNA_TRACE_FUNCTION();

  /* The hints to pass for address information lookup. */
  struct addrinfo hints;

  /* The result from the address info lookup. */
  struct addrinfo *result = nullptr;

  /* The address family. */
  int family = (version == IPAddress::Versions::V4 ? AF_INET : AF_INET6);

  /* Clear the hints object. */
  memset(&hints, 0, sizeof(struct addrinfo));

  /* Set the domain hint. */
  hints.ai_family = family;
  hints.ai_flags = AI_PASSIVE;

  /* Retrieve the address list. */
  int ret = getaddrinfo(host.c_str(), nullptr, &hints, &result);

  /* Check if the operation failed. */
  if(ret != 0)
  {
    LUNA_THROW_RUNTIME_ERROR("getaddrinfo() failed with error: " << ret);
  }

  /* Create a unique pointer to free the resources when done. */
  std::unique_ptr<struct addrinfo, decltype(&freeaddrinfo)> addrInfo(
      result, freeaddrinfo);

  /* The list of IP addresses. */
  std::vector<IPAddress> addresses;

  /* Get all the IP addresses. */
  while(result)
  {
    /* Make sure the IP address is the right family / domain. */
    if(result->ai_family == AF_INET)
    {
      /* Cast the address structure to the IPv4 version. */
      auto addrIn = reinterpret_cast<struct sockaddr_in *>(result->ai_addr);

      /* Build the IP Address object. */
      auto ipAddr =
          IPAddress(reinterpret_cast<uint8_t *>(&(addrIn->sin_addr)), version);

      /* Check to see if it's allready in the list. I.e. by default multiple
       * instance of an IP address will be returned for the various supported
       * protocols. */
      if(std::find(addresses.begin(), addresses.end(), ipAddr) ==
          addresses.end())
      {
        /* Create the new IP address and add it the array of IP addresses. */
        addresses.push_back(ipAddr);
      }
    }
    else
    {
      /* Cast the address strcuture to the IPv6 version. */
      auto addrIn = reinterpret_cast<struct sockaddr_in6 *>(result->ai_addr);

      /* Build the IP Address. */
      auto ipAddr =
          IPAddress(reinterpret_cast<uint8_t *>(&(addrIn->sin6_addr)), version);

      /* Check to see if it's allready in the list. I.e. by default multiple
       * instance of an IP address will be returned for the various supported
       * protocols. */
      if(std::find(addresses.begin(), addresses.end(), ipAddr) ==
          addresses.end())
      {
        /* Create the new IP address and add it the array of IP addresses. */
        addresses.push_back(ipAddr);
      }
    }

    /* Advance to the next result in the queue. */
    result = result->ai_next;
  }

  /* Return the list of addresses. */
  return addresses;
}

/******************************************************************************/
std::string DNS::resolve(const IPAddress &addr)
{
  /* Trace the function to the log file. */
  LUNA_TRACE_FUNCTION();

  LUNA_THROW_RUNTIME_ERROR("Function not implemented.");
}
