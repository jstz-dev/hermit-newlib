#ifndef NETDB_H
#define NETDB_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define socklen_t __socklen_t

typedef struct addrinfo_t {
  int32_t ai_flags;
  int32_t ai_family;
  int32_t ai_socktype;
  int32_t ai_protocol;
  socklen_t ai_addrlen;
  struct sockaddr *ai_addr;
  uint8_t *ai_canonname;
  struct addrinfo_t *ai_next;
} addrinfo;

struct in_addr {
    unsigned long s_addr;  // load with inet_aton()
};

struct sockaddr_in {
    short            sin_family;   // e.g. AF_INET
    unsigned short   sin_port;     // e.g. htons(3490)
    struct in_addr   sin_addr;     // see struct in_addr, below
    char             sin_zero[8];  // zero this if you want to
};

#ifdef __cplusplus
}
#endif

#endif /* NETDB_H */