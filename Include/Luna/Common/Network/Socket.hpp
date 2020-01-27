#pragma once

#include <memory>
#include <optional>
#include <vector>

#include "../../Config.hpp"
#include "SocketAddress.hpp"

namespace Luna
{
  /**
   * A simple wrapper / abstraction class for Unix / Windows sockets. It only
   * support TCP, UDP and Unix sockets. Unix sockets (AF_UNIX) is now supported
   * in Windows from Windows build 17061.
   *
   * 1. Download the Windows Insiders SDK for the Windows build 17061.
   * 2. Check whether your Windows build has support for unix socket by running
   *    “sc query afunix” from a Windows admin command prompt.
   * 3. #include <afunix.h> in your Windows application and write a Windows
   *    unix socket winsock application as you would write any other unix
   *    socket application, but, using Winsock API’s.
   *
   * Example 1 - Unix Socket Server
   * ------------------------------
   * auto soc = Socket(Unix, Stream);
   * soc.bind("/tmp/unix_sock_test");
   * soc.read(...);
   * soc.write(...);
   *
   * Example 2 - Unix Socket Client
   * ------------------------------
   * auto soc = Socket(Unix, Stream);
   * soc.connect("/tmp/unix_sock_test");
   * soc.read(...);
   * soc.write(...);
   *
   */
  class Socket
  {
  public:
    enum class Types
    {
      Stream,
      Datagram
    };

    /**
     * Create a new socket of the specified domain and type.
     * @param domain  The domain of the socket. Use Unix for IPC (now supported
     *                on Windows as well apparently). Else use IPv4 or IPv6
     *                for internet traffic. In the case of a sever that should
     *                accept both IPv4 and IPv6 conenction, use Any.
     * @param type    The type of the socket. Use Stream for byte arrays
     without
     *                boundaries or Datagram for blocked based data.
     */
    Socket(SocketAddress::Domains domain, Types type);

    /**
     * Bind the server to the specified address. Though you can call bind on
     * the client, this is generally considered a bad idea.l
     */
    void bind(const SocketAddress &addr);

    /**
     * Listen for new client connection requests. This is only used for Unix
     * and Stream (TCP) Sockets.
     *
     * @param backlog The maximum number of concurrent connetions to queue.
     * @return        The socket used to communicate with the client.
     */
    std::unique_ptr<Socket> listen(size_t backlog);

    /**
     * Connect to the specified server address and port.
     *
     * @param addr  The IP address of the server.
     * @param port  The port number on the server.
     * @return      True if the socket connected, or false if not.
     */
    bool connect(const SocketAddress &addr);

    virtual ~Socket();

    virtual bool isReliable() const;

    virtual Types type() const;

    /**
     * Return a list of IP addresses that maps to the specified host or domain
     * name.
     *
     * @param name  The host name / domain name to lookup.
     * @return
     */
    std::vector<std::string> getAddresses(
        const std::string &name, Domains domain);

  protected:
    /** The platform specific data that is used to implement socket
     * functionality. */
    struct Data;

    const Domains kDomain;

    const Types kType;

    /** The isntance for the platform specific data. */
    std::unique_ptr<Data> fData;

    /** Create a new socket with the specified data. This is only available
     * for member functions suchs as listen() and the Data struct. */
    Socket(std::unique_ptr<Data> data, Domains domain, Types type);
  };
} /* namespace Luna */
