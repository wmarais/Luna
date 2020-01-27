#pragma once

#include <memory>

namespace Luna
{
  /**
   * An wrapped around the OS in_addr and in6_addr types with nice abstraction
   * functions to make it a bit easier and safer to use than the C equivalents.
   */
  class IPAddress final
  {

  public:
    /** The diffirent IP versions supported. */
    enum Versions
    {
      /* An IPv4 Address. */
      V4,

      /** And IPv6 Address. */
      V6,

      /** Use Any to listen for both IPv4 and IPv6 conenctions. Used for
       * servers, though this behaviour can be overwritten by the OS. */
      Any
    };

    /**
     * Create a new IP Address object from the supplied IP string. This Will
     * automatically detect the type of IP address based on the presence of
     * either a '.' or ':' seperator.
     *
     * @param ip    The IP address string, i.e. 127.0.0.1, or ::1, etc.
     */
    IPAddress(const std::string &ip);

    /**
     * Copy the specified IP address.
     *
     * @param cp  The IP Address to copy.
     */
    IPAddress(const IPAddress &cp);

    /**
     * The version protocol version of the IP Address.
     *
     * @return  The protocl version of the IP Address.
     */
    Versions version() const;

    /**
     * Convert the IP address into a string that can be used for comparison and
     * printing.
     *
     * @return  The string representation of the IP Address.
     */
    std::string toString() const;

    /**
     * Compare this IP address to the supplied one and check if they are equal.
     * The IP addresses are converted to strings and then compared.
     *
     * @param ip  The address to compare too.
     * @return    True if the IP addresses matches, else False.
     */
    bool operator==(const IPAddress &ip) const;

    /**
     * Return the pointer to the "struct in_addr" or "struct in6_addr" depending
     * of whether it is an IPv4 or IPv6 socket.
     *
     * @return  The pointer to the address data.
     */
    uint8_t *data() const;

  private:
    /** Indicate whether the address is IPv4 or IPv6. */
    Versions fVersion;

    /** The IP Address structure stored as a byte array. */
    std::unique_ptr<uint8_t[]> fData;

    /**
     * Private constructor to use by the DNS and SocketAddress classes to create
     * IP Addresses.
     *
     * @param data      The "struct in_addr" or "struct in6_addr" memory.
     * @param version   The IP version to determine is an "in_addr" or
     *                  "in6_addr" memory block has been provided in data.
     */
    IPAddress(const uint8_t *data, Versions version);

    /** Allow the DNS Class to acce4ss the private constructor. */
    friend class DNS;

    /** Allow the SocketAddress structure to access the private constructor. */
    friend class SocketAddress;
  };

  std::ostream &operator<<(std::ostream &os, const IPAddress &addr);

} /* namespace Luna */
