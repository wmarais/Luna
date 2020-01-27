//#include "../../../../Include/Luna/Common/Network/Socket.hpp"
//#include "../../../../Include/Luna/Common/Debug/Exception.hpp"
//#include "../../../../Include/Luna/Common/SystemHeaders.hpp"

// using namespace Luna;

// struct Socket::Data
//{
//  Luna::socket_t fHandle;

//  int domToInt(Domains dom)
//  {
//    if(dom == Domains::IPv4)
//      return AF_INET;

//    if(dom == Domains::IPv6)
//      return AF_INET6;

//#ifdef LUNA_HAS_AFUNIX_H
//    return AF_INET6;
//#else
//    LUNA_THROW_RUNTIME_ERROR("Unix socket support is not available.");
//#endif /* LUNA_HAS_AFUNIX_H */
//  }

//  int typeToInt(Types type)
//  {
//    if(type == Types::Stream)
//      return SOCK_STREAM;

//    return SOCK_DGRAM;
//  }

//  Data(Luna::socket_t handle) : fHandle(handle) {}

//  Data(Domains domain, Types type)
//  {
//    fHandle = socket(domToInt(domain), typeToInt(type), 0);

//    if(fHandle < 0)
//    {
//      LUNA_THROW_RUNTIME_ERROR(
//          "Failed to create socket because: " << strerror(errno));
//    }
//  }
//};

///******************************************************************************/
// Socket::Socket(Domains domain, Types type) : kDomain(domain), kType(type)
//{
//  /* Create the socket data. */
//  fData = std::make_unique<Data>(domain, type);
//}

///******************************************************************************/
// Socket::Socket(std::unique_ptr<Data> data, Domains domain, Types type)
//    : kDomain(domain), kType(type)
//{
//  fData = std::move(data);
//}

///******************************************************************************/
// std::unique_ptr<Socket> Socket::listen(size_t backlog)
//{
//  /* Check for a new socket request. */
//  Luna::socket_t handle = ::listen(fData->fHandle, static_cast<int>(backlog));

//  /* Check if the connection failed. */
//  if(handle == LUNA_INVALID_SOCKET)
//  {
//    LUNA_THROW_RUNTIME_ERROR(
//        "Socket connection request / listen() failed because: " << strerror(
//            errno));
//  }
//  /* Create the socket data. */
//  auto data = std::make_unique<Data>(handle);

//  /* Create and return the socket. */
//  return std::unique_ptr<Socket>(new Socket(std::move(data), kDomain, kType));
//}

///******************************************************************************/
// bool Socket::connect(const std::string &address, uint16_t port)
//{

//  /* Check if it's a Unix Socket. */
//  if(kDomain == Domains::Unix)
//  {
//    /* Initialise the address structure. */
//    struct sockaddr_un addr;
//    memset(&addr, 0, sizeof(struct sockaddr_un));
//    addr.sun_family = AF_UNIX;
//    strncpy(addr.sun_path, address.c_str(), sizeof(addr.sun_path) - 1);

//    /* Connect the socket and check if an error occured. */
//    if(::connect(fData->fHandle, reinterpret_cast<struct sockaddr *>(&addr),
//           sizeof(struct sockaddr_un)) == -1)
//    {
//      LUNA_THROW_RUNTIME_ERROR(
//          "Failed to connect unix socket:" << strerror(errno));
//    }
//  }
//  // else if(kDomain == Domains::)
//}

///******************************************************************************/
// std::vector<std::string> Socket::getAddresses(
//    const std::string &name, Domains domain)
//{
//}
