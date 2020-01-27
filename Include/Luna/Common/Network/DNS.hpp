#pragma once

#include "IPAddress.hpp"
#include <string>
#include <vector>

namespace Luna
{
  class DNS final
  {
  public:
    /**
     * Resolve all the IP addresses of the specified version that maps to the
     * host or domain name supplied.
     *
     * @param host      The host or domain name to resolve.
     * @param version   The IP address versions of interest.
     * @return          The list of resolved IP addresses.
     */
    static std::vector<IPAddress> resolve(
        const std::string &host, IPAddress::Versions version);

    static std::string resolve(const IPAddress &addr);
  };
} /* namespace Luna */
