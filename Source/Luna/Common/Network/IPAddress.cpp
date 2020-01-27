#include "../../../../Include/Luna/Common/Network/IPAddress.hpp"
#include "../../../../Include/Luna/Common/Debug/Exception.hpp"
#include "../../../../Include/Luna/Common/Debug/FunctionTracer.hpp"
#include "../../../../Include/Luna/Common/Debug/Log.hpp"
#include "../../../../Include/Luna/Common/SystemHeaders.hpp"

using namespace Luna;

/******************************************************************************/
IPAddress::IPAddress(const std::string &ip)
{
  /* Trace the function to the log file. */
  LUNA_TRACE_FUNCTION();

  /* First check whether it is an IPv4 Address. */
  if(ip.find(".") != std::string::npos)
  {
    /* Set the version as 4. */
    fVersion = Versions::V4;

    /* Allocate the space for the address. */
    fData = std::make_unique<uint8_t[]>(sizeof(Luna::ipv4_addr_t));

    /* Set the value. */
    auto addr = reinterpret_cast<Luna::ipv4_addr_t *>(fData.get());
    memset(addr, 0, sizeof(Luna::ipv4_addr_t));
    if(inet_pton(AF_INET, ip.data(), addr) != 1)
    {
      LUNA_THROW_RUNTIME_ERROR("Invalid IPv4 address supplied: " << ip);
    }
  }
  /* Check if it's an IPv6 Address. */
  else if(ip.find(":") != std::string::npos)
  {
    /* Set the version as 6. */
    fVersion = Versions::V6;

    /* Allocate the space for the address. */
    fData = std::make_unique<uint8_t[]>(sizeof(Luna::ipv6_addr_t));

    /* Set the value. */
    auto addr = reinterpret_cast<Luna::ipv6_addr_t *>(fData.get());
    memset(addr, 0, sizeof(Luna::ipv6_addr_t));
    if(inet_pton(AF_INET6, ip.data(), addr) != 1)
    {
      LUNA_THROW_RUNTIME_ERROR("Invalid IPv6 address supplied: " << ip);
    }
  }
  /* Otherwise some type of grabage was supplied. */
  else
  {
    LUNA_THROW_RUNTIME_ERROR("Unknown IP Address version: " << ip);
  }
}

/******************************************************************************/
IPAddress::IPAddress(const IPAddress &cp) : fVersion(cp.fVersion)
{
  /* Trace the function to the log file. */
  LUNA_TRACE_FUNCTION();

  if(cp.fVersion == Versions::V4)
  {
    fData = std::make_unique<uint8_t[]>(sizeof(Luna::ipv4_addr_t));
    memcpy(fData.get(), cp.fData.get(), sizeof(Luna::ipv4_addr_t));
  }
  else if(cp.fVersion == Versions::V6)
  {
    fData = std::make_unique<uint8_t[]>(sizeof(Luna::ipv6_addr_t));
    memcpy(fData.get(), cp.fData.get(), sizeof(Luna::ipv6_addr_t));
  }
  else
  {
    LUNA_THROW_RUNTIME_ERROR("Unknown IP Address version being copied.");
  }
}

/******************************************************************************/
IPAddress::IPAddress(const uint8_t *data, Versions version) : fVersion(version)
{
  /* Trace the function to the log file. */
  LUNA_TRACE_FUNCTION();

  if(version == Versions::V4)
  {
    /* Allocate the required memory. */
    fData = std::make_unique<uint8_t[]>(sizeof(Luna::ipv4_addr_t));
    memset(fData.get(), 0, sizeof(Luna::ipv4_addr_t));
    memcpy(fData.get(), data, sizeof(Luna::ipv4_addr_t));
  }
  else
  {
    /* Allocate the required memory. */
    fData = std::make_unique<uint8_t[]>(sizeof(Luna::ipv6_addr_t));
    memset(fData.get(), 0, sizeof(Luna::ipv6_addr_t));
    memcpy(fData.get(), data, sizeof(Luna::ipv6_addr_t));
  }
}

/******************************************************************************/
IPAddress::Versions IPAddress::version() const
{
  /* Trace the function to the log file. */
  LUNA_TRACE_FUNCTION();

  return fVersion;
}

/******************************************************************************/
std::string IPAddress::toString() const
{
  /* Trace the function to the log file. */
  LUNA_TRACE_FUNCTION();

  /* The IP address as a string. */
  std::string addrStr;

  /* Check if it's an IPv4 Address. */
  if(fVersion == Versions::V4)
  {
    /* Resize to fit the address. */
    addrStr.resize(INET_ADDRSTRLEN);

    /* Cast the socket address data to the correct type. */
    auto addr = reinterpret_cast<ipv4_addr_t *>(fData.get());

    /* Convert the address. */
    inet_ntop(AF_INET, &(addr->s_addr), addrStr.data(), INET_ADDRSTRLEN);
  }
  else
  {
    /* Resize to fit the address. */
    addrStr.resize(INET6_ADDRSTRLEN);

    /* Cast the socket address data to the correct type. */
    auto addr = reinterpret_cast<ipv6_addr_t *>(fData.get());

    /* Convert the address. */
    inet_ntop(AF_INET6, addr, addrStr.data(), INET6_ADDRSTRLEN);
  }

  /* Return the string of the IP address. */
  return addrStr;
}

/******************************************************************************/
uint8_t *IPAddress::data() const
{
  /* Trace the function to the log file. */
  LUNA_TRACE_FUNCTION();

  return fData.get();
}

/******************************************************************************/
bool IPAddress::operator==(const IPAddress &ip) const
{
  /* Trace the function to the log file. */
  LUNA_TRACE_FUNCTION();

  return toString() == ip.toString();
}

/******************************************************************************/
std::ostream &Luna::operator<<(std::ostream &os, const IPAddress &addr)
{
  os << addr.toString();
  return os;
}
