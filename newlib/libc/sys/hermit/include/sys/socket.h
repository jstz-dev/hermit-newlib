#ifndef SOCKET_H
#define SOCKET_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>
#include <netinet/in.h>
#include <netdb.h>

/* Protocol families, same as address families. */
#define PF_INET AF_INET

// Define socket address structure
#define sa_family_t __sa_family_t

typedef struct sockaddr {
  uint8_t sa_len;
  uint8_t sa_family;
  uint8_t sa_data[14];
};

// Define POSIX function names
#define socket                sys_socket
#define accept                sys_accept
#define listen                sys_listen
#define bind                  sys_bind
#define connect               sys_connect
#define getsockname           sys_getsockname
#define setsockopt            sys_setsockopt
#define getpeername           sys_getpeername
#define freeaddrinfo          sys_freeaddrinfo
#define getaddrinfo           sys_getaddrinfo
#define send                  sys_send
#define shutdown              sys_shutdown
#define recv                  sys_recv
#define sendto                sys_sendto
#define recvfrom              sys_recvfrom

#define SockType uint32_t


// Define socket API functions
int32_t sys_socket(int32_t domain, SockType type_, int32_t protocol);
int32_t sys_accept(int32_t s, struct sockaddr *addr, socklen_t *addrlen);
int32_t sys_listen(int32_t s, int32_t backlog);
int32_t sys_bind(int32_t s, const struct sockaddr *name, socklen_t namelen);
int32_t sys_connect(int32_t s, const struct sockaddr *name, socklen_t namelen);
int32_t sys_getsockname(int32_t s, struct sockaddr *name, socklen_t *namelen);
int32_t sys_setsockopt(int32_t s,
                       int32_t level,
                       int32_t optname,
                       const void *optval,
                       socklen_t optlen);
int32_t getsockopt(int32_t s, int32_t level, int32_t optname, void *optval, socklen_t *optlen);
int32_t sys_getpeername(int32_t s, struct sockaddr *name, socklen_t *namelen);
void sys_freeaddrinfo(addrinfo *ai);
int32_t sys_getaddrinfo(const uint8_t *nodename,
                        const uint8_t *servname,
                        const addrinfo *hints,
                        addrinfo **res);
intptr_t sys_send(int32_t s, const void *mem, uintptr_t len, int32_t _flags);
int32_t sys_shutdown(int32_t s, int32_t how);
intptr_t sys_recv(int32_t fd, uint8_t *buf, uintptr_t len, int32_t flags);
intptr_t sys_sendto(int32_t socket,
                    const uint8_t *buf,
                    uintptr_t len,
                    int32_t flags,
                    const struct sockaddr *addr,
                    socklen_t addrlen);
intptr_t sys_recvfrom(int32_t socket,
                      uint8_t *buf,
                      uintptr_t len,
                      int32_t flags,
                      struct sockaddr *addr,
                      socklen_t *addrlen);

// Defines from librs
#define AF_INET 0
#define AF_INET6 1

#define IPPROTO_IP 0
#define IPPROTO_IPV6 41
#define IPPROTO_TCP 6
#define IPPROTO_UDP 17

#define IPV6_ADD_MEMBERSHIP 12
#define IPV6_DROP_MEMBERSHIP 13
#define IPV6_MULTICAST_LOOP 19
#define IPV6_V6ONLY 27

#define IP_TTL 2
#define IP_MULTICAST_TTL 5
#define IP_MULTICAST_LOOP 7
#define IP_ADD_MEMBERSHIP 3
#define IP_DROP_MEMBERSHIP 4

#define SHUT_RD 0
#define SHUT_WR 1
#define SHUT_RDWR 2

#define SOCK_DGRAM 2
#define SOCK_STREAM 1

#define SOL_SOCKET 4095

#define SO_BROADCAST 32
#define SO_ERROR 4103
#define SO_RCVTIMEO 4102
#define SO_REUSEADDR 4
#define SO_SNDTIMEO 4101
#define SO_LINGER 128

#define TCP_NODELAY 1
#define MSG_PEEK 1
#define FIONBIO (int32_t)2148034174u

#define EAI_NONAME -2200
#define EAI_SERVICE -2201
#define EAI_FAIL -2202
#define EAI_MEMORY -2203
#define EAI_FAMILY -2204

// Seperate defines
#define SO_SNDBUF       0x1001
#define SO_RCVBUF       0x1002


#define FD_BIT	      (1 << 30)
#define FD_SETSIZE    32
#define FD_SET(n, p)  ((p)->fd_bits[((n) & ~FD_BIT)/8] |=  (1 << (((n) & ~FD_BIT) & 7)))
#define FD_CLR(n, p)  ((p)->fd_bits[((n) & ~FD_BIT)/8] &= ~(1 << (((n) & ~FD_BIT) & 7)))
#define FD_ISSET(n,p) ((p)->fd_bits[((n) & ~FD_BIT)/8] &   (1 << (((n) & ~FD_BIT) & 7)))
#define FD_ZERO(p)    memset((void*)(p),0,sizeof(*(p)))

typedef struct fd_set {
	unsigned char fd_bits [(FD_SETSIZE+7)/8];
} fd_set;

#include <arpa/inet.h>

#ifdef __cplusplus
}
#endif

#endif /* SOCKET_H */