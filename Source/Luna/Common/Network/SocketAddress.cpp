#include "../../../../Include/Luna/Common/Network/SocketAddress.hpp"
#include "../../../../Include/Luna/Common/Debug/Exception.hpp"
#include "../../../../Include/Luna/Common/Debug/FunctionTracer.hpp"
#include "../../../../Include/Luna/Common/Debug/Log.hpp"
#include "../../../../Include/Luna/Common/SystemHeaders.hpp"
#include <string.h>

using namespace Luna;

/******************************************************************************/
SocketAddress::SocketAddress(const IPAddress &address, uint16_t port)
{
  /* Trace the function to the log file. */
  LUNA_TRACE_FUNCTION();

  /* Create the socket address object. */
  fData = std::make_unique<uint8_t[]>(sizeof(struct sockaddr));

  /* Clear the structure. */
  memset(fData.get(), 0, sizeof(struct sockaddr));

  /* Check if it's an IPv4 Address. */
  if(address.version() == IPAddress::Versions::V4)
  {
    /* Cast the structure to the INET version. */
    auto addrIn = reinterpret_cast<struct sockaddr_in *>(fData.get());
    addrIn->sin_family = AF_INET;
    addrIn->sin_port = port;
    memcpy(&(addrIn->sin_addr), address.data(), sizeof(Luna::ipv4_addr_t));
  }
  else
  {
    /* Cast the structure to the INET version. */
    auto addrIn6 = reinterpret_cast<struct sockaddr_in6 *>(fData.get());
    addrIn6->sin6_family = AF_INET6;
    addrIn6->sin6_port = port;
    memcpy(&(addrIn6->sin6_addr), address.data(), sizeof(Luna::ipv6_addr_t));
  }
}

/******************************************************************************/
SocketAddress::SocketAddress(const FilePath &path)
{
  /* Trace the function to the log file. */
  LUNA_TRACE_FUNCTION();

  /* Create the socket address object. */
  fData = std::make_unique<uint8_t[]>(sizeof(struct sockaddr));

  /* Clear the structure. */
  memset(fData.get(), 0, sizeof(struct sockaddr));

  auto addrUn = reinterpret_cast<struct sockaddr_un *>(fData.get());
  addrUn->sun_family = AF_UNIX;
  strncpy(
      addrUn->sun_path, path.toString().c_str(), sizeof(addrUn->sun_path) - 1);
}

/******************************************************************************/
SocketAddress::Domains SocketAddress::domain() const
{
  /* Trace the function to the log file. */
  LUNA_TRACE_FUNCTION();

  /* Get the pointer to the socket address. */
  auto addr = reinterpret_cast<struct sockaddr *>(fData.get());

  /* Check what it is. */
  switch(addr->sa_family)
  {
  case AF_INET:
    return Domains::Inet;
  case AF_INET6:
    return Domains::Inet6;
  case AF_UNIX:
    return Domains::Unix;
  default:
    LUNA_THROW_RUNTIME_ERROR("Unsupported socket domain: " << addr->sa_family);
  }
}

/******************************************************************************/
IPAddress SocketAddress::ip() const
{
  /* Trace the function to the log file. */
  LUNA_TRACE_FUNCTION();

  /* Get the pointer to the socket address. */
  auto addr = reinterpret_cast<struct sockaddr *>(fData.get());

  /* Check  if it's a IPv4 address. */
  if(addr->sa_family == AF_INET)
  {
    auto addrIn = reinterpret_cast<struct sockaddr_in *>(fData.get());
    return IPAddress(reinterpret_cast<uint8_t *>(&(addrIn->sin_addr)),
        IPAddress::Versions::V4);
  }

  /* Check if it's an IPv6 Address. */
  if(addr->sa_family == AF_INET6)
  {
    auto addrIn6 = reinterpret_cast<struct sockaddr_in6 *>(fData.get());
    return IPAddress(reinterpret_cast<uint8_t *>(&(addrIn6->sin6_addr)),
        IPAddress::Versions::V6);
  }

  /* The socket does not have an IP address. */
  LUNA_THROW_RUNTIME_ERROR(
      "The Socket Address is not for an Inet or Inet6 Socket!");
}

/******************************************************************************/
uint16_t SocketAddress::port() const
{
  /* Trace the function to the log file. */
  LUNA_TRACE_FUNCTION();

  /* Get the pointer to the socket address. */
  auto addr = reinterpret_cast<struct sockaddr *>(fData.get());

  /* Check  if it's a IPv4 address. */
  if(addr->sa_family == AF_INET)
  {
    auto addrIn = reinterpret_cast<struct sockaddr_in *>(fData.get());
    return addrIn->sin_port;
  }

  /* Check if it's an IPv6 Address. */
  if(addr->sa_family == AF_INET6)
  {
    auto addrIn6 = reinterpret_cast<struct sockaddr_in6 *>(fData.get());
    return addrIn6->sin6_port;
  }

  /* The socket does not have an IP address. */
  LUNA_THROW_RUNTIME_ERROR(
      "The Socket Address is not for an Inet or Inet6 Socket!");
}

/******************************************************************************/
FilePath SocketAddress::path() const
{
  /* Trace the function to the log file. */
  LUNA_TRACE_FUNCTION();

  /* Get the pointer to the socket address. */
  auto addr = reinterpret_cast<struct sockaddr *>(fData.get());

  /* Check if it's a Unix Socket address. */
  if(addr->sa_family == AF_UNIX)
  {
    auto addrUn = reinterpret_cast<struct sockaddr_un *>(fData.get());
    return FilePath(addrUn->sun_path);
  }

  /* It's nto a unix socket and does not have a path. */
  LUNA_THROW_RUNTIME_ERROR("The Socket Address is not for a Unix Socket!");
}
