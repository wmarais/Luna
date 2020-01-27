#pragma once

#include "../Config.hpp"

/* Windows have very touchy-feely include orders and defitions. So always leave
 * it at the top of the include list and retain the header inclusions as listed
 * below. Changes to this will stop some things from building. */
#ifdef LUNA_OS_WINDOWS

/* Reduce the windows footprint. */
#define WIN32_LEAN_AND_MEAN

#include <afunix.h>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#ifdef LUNA_HAS_AFUNIX_H
#include <afunix.h>
#endif /* LUNA_HAS_AFUNIX_H */

namespace Luna
{
  typedef SOCKET socket_t;
}

#define LUNA_INVALID_SOCKET INVALID_SOCKET

#endif /* LUNA_OS_WINDOWS */

/* All the linux specific headers go here. Less senstive than the windows
 * headers. */
#ifdef LUNA_OS_LINUX
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <syslog.h>
#include <unistd.h>

namespace Luna
{
  typedef int socket_t;

  typedef struct in_addr ipv4_addr_t;
  typedef struct in6_addr ipv6_addr_t;
} // namespace Luna

#define LUNA_INVALID_SOCKET -1

#endif /* LUNA_OS_LINUX */

#include <error.h>
#include <string.h>
