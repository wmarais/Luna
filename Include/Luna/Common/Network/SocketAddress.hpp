#pragma once

#include "../FileSystem/FilePath.hpp"
#include "IPAddress.hpp"
#include <cstdint>
#include <memory>

namespace Luna
{
  /**
   * A simple wrapper class around "struct sockaddr" that internally cast to
   * "struct sockaddr_in" for AF_INET, "struct sockaddr_in6" AF_INET6 and
   * "struct sockaddr_un" for AF_UNIX.
   *
   * Unfortunately because some libraries are weird about the order in which
   * headers are included, the "struict sockaddr" is not stored type safe, but
   * rather as a byte array. */
  class SocketAddress final
  {
  public:
    /** List of Socket Domains. */
    enum Domains
    {
      /** Internet (IPv4) domain. */
      Inet,
      /** Internet (IPv6) domain. */
      Inet6,

      /** Unix (Local) domain. */
      Unix
    };

    /**
     * Create an address that is used for Unix Sockets.
     *
     * @param path  The path on the file system.
     */
    SocketAddress(const FilePath &path);

    /**
     * Create an address that is sued for IPv4 and IPv6 Sockets.
     *
     * @param addr  The IP address of the local or remote socket.
     * @param port  The port of the local or remote socket.
     */
    SocketAddress(const IPAddress &addr, uint16_t port);

    /**
     * Check which domain the socket address belong too, either Inet (IPv4),
     * Inet6 (IPv6) and Unix (local).
     *
     * @return  The domain of the socket.
     */
    Domains domain() const;

    /**
     * Return the IP address of the socket for Inet and Inet6 sockets. An
     * exception will be thrown if called on a Unix Socket Address.
     *
     * @return  The ip address of the socket.
     */
    IPAddress ip() const;

    /**
     * The port number of the socket address. This is only valid for Inet and
     * Inet6, and calling it on a Unix Socket Address object will throw an
     * exception.
     * @return
     */
    uint16_t port() const;

    /**
     * Return the Path of a the socket address if it's a Unix / Local socket.
     *
     * @return  The path of the unix socket.
     */
    FilePath path() const;

  private:
    /** The "struct sockaddr" data structure memory. */
    std::unique_ptr<uint8_t[]> fData;
  };
} /* namespace Luna */
