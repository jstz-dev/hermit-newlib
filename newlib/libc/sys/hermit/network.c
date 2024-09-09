// SPDX-FileCopyrightText: 2024 TriliTech <contact@trili.tech>
// SPDX-FileCopyrightText: 2005-2014 Rich Felker, et al.
// SPDX-License-Identifier: MIT

#define _GNU_SOURCE
#include <reent.h>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <ctype.h>
#include <byteswap.h>
#include <hermit.h>

// Note: This file has alot missing from it, but it is a good starting point for networking support

HERMIT_SYSCALL(int, socket, (int domain, int type, int protocol), (domain, type, protocol))
HERMIT_STUB(int, socketpair, (int domain, int type, int protocol, int fd[2]), -1)

HERMIT_SYSCALL(int, listen, (int fd, int backlog), (fd, backlog))
HERMIT_SYSCALL(int, bind, (int fd, const struct sockaddr *addr, socklen_t len), (fd, addr, len))
HERMIT_SYSCALL(int, connect, (int fd, const struct sockaddr *addr, socklen_t len), (fd, addr, len))

HERMIT_SYSCALL(int, shutdown, (int fd, int how), (fd, how))

HERMIT_SYSCALL(int, accept, (int fd, struct sockaddr *addr, socklen_t *len), (fd, addr, len))
HERMIT_STUB(int, accept4, (int fd, struct sockaddr *addr, socklen_t *len, int flags), -1)

HERMIT_STUB(int, sendmmsg, (int fd, struct mmsghdr *msgvec, unsigned int vlen, unsigned int flags), -1)
HERMIT_STUB(int, recvmmsg, (int fd, struct mmsghdr *msgvec, unsigned int vlen, unsigned int flags, struct timespec *timeout), -1)

HERMIT_SYSCALL(int, getsockname, (int fd, struct sockaddr *addr, socklen_t *len), (fd, addr, len))
HERMIT_SYSCALL(int, getpeername, (int fd, struct sockaddr *addr, socklen_t *len), (fd, addr, len))

HERMIT_SYSCALL(ssize_t, send, (int fd, const void *buf, size_t len, int flags), (fd, buf, len, flags))
HERMIT_SYSCALL(ssize_t, sendto, (int fd, const void *buf, size_t len, int flags, const struct sockaddr *addr, socklen_t alen), (fd, buf, len, flags, addr, alen))

HERMIT_SYSCALL(ssize_t, recv, (int fd, void *buf, size_t len, int flags), (fd, buf, len, flags))
HERMIT_SYSCALL(ssize_t, recvfrom, (int fd, void *buf, size_t len, int flags, struct sockaddr *addr, socklen_t *alen), (fd, buf, len, flags, addr, alen))

HERMIT_STUB(ssize_t, sendmsg, (int fd, const struct msghdr *msg, int flags), -1)
HERMIT_STUB(ssize_t, recvmsg, (int fd, struct msghdr *msg, int flags), -1)

HERMIT_SYSCALL(int, getsockopt, (int fd, int level, int optname, void *optval, socklen_t *optlen), (fd, level, optname, optval, optlen))
HERMIT_SYSCALL(int, setsockopt, (int fd, int level, int optname, const void *optval, socklen_t optlen), (fd, level, optname, optval, optlen))

HERMIT_STUB(int, sockatmark, (int fd), -1)

int getnameinfo(const struct sockaddr *restrict sa, socklen_t sl,
                char *restrict node, socklen_t nodelen,
                char *restrict serv, socklen_t servlen,
                int flags)
{
  char buf[256];
  /*unsigned char reply[512]; TODO used in DNS reply */
  int af = sa->sa_family;
  char line[512];
  FILE *f;
  unsigned char *a;

  switch (af)
  {
  case AF_INET:
    a = (void *)&((struct sockaddr_in *)sa)->sin_addr;
    if (sl != sizeof(struct sockaddr_in))
      return EAI_FAMILY;
    break;
#if CONFIG_LWIP_IPV6
  case AF_INET6:
    a = (void *)&((struct sockaddr_in6 *)sa)->sin6_addr;
    if (sl != sizeof(struct sockaddr_in6))
      return EAI_FAMILY;
    break;
#endif
  default:
    return EAI_FAMILY;
  }

  /* Try to find ip within /etc/hosts */
  if ((node && nodelen) && (af == AF_INET))
  {
    const char *ipstr;
    size_t l;

    ipstr = inet_ntoa(((struct sockaddr_in *)sa)->sin_addr);
    l = strlen(ipstr);
    f = fopen("/etc/hosts", "r");
    if (f)
      while (fgets(line, sizeof(line), f))
      {
        char *domain;

        if (strncmp(line, ipstr, l) != 0)
          continue;

        domain = strtok(line, " ");
        if (!domain)
          continue;
        domain = strtok(NULL, " ");
        if (!domain)
          continue;

        if (strlen(domain) >= nodelen)
          return EAI_OVERFLOW;
        strcpy(node, domain);
        fclose(f);
        return 0;
      }
    if (f)
      fclose(f);
  }

  if (node && nodelen)
  {
    if ((flags & NI_NUMERICHOST)
#if 0
			/* TODO, Hermit doesn't currently don't support name requests */
			|| __dns_query(reply, a, af, 1) <= 0
			|| __dns_get_rr(buf, 0, 256, 1, reply, RR_PTR, 1) <= 0) {
#else
        || 1)
    {
#endif
      if (flags & NI_NAMEREQD)
        return EAI_NONAME;
      inet_ntop(af, a, buf, sizeof(buf));
    }
    if (strlen(buf) >= nodelen)
      return EAI_OVERFLOW;
    strcpy(node, buf);
  }

  if (serv && servlen)
  {
    if (snprintf(buf, sizeof(buf), "%d",
                 ntohs(((struct sockaddr_in *)sa)->sin_port)) >= (int)servlen)
      return EAI_OVERFLOW;
    strcpy(serv, buf);
  }

  return 0;
}

HERMIT_STUB(struct hostent *, gethostbyaddr, (const void *addr, socklen_t len, int type), NULL)

HERMIT_SYSCALL(int, getaddrinfo, (const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res), (node, service, hints, res))

void freeaddrinfo(struct addrinfo *res)
{
  return sys_freeaddrinfo(res);
}

const char *gai_strerror(int errcode)
{
  switch (errcode)
  {
  default:
    return "Error on getaddrinfo.";
  }
}

void sethostent(int x)
{
}

struct hostent *gethostent()
{
  return 0;
}

void endhostent(void)
{
}

HERMIT_STUB(int, gethostname, (char *name, size_t len), -1)
HERMIT_STUB(int, sethostname, (const char *name, size_t len), -1)

int h_errno;

int *__h_errno_location(void)
{
  return __errno();
}

HERMIT_STUB(struct hostent *, gethostbyname, (const char *name), NULL)
HERMIT_STUB(struct hostent *, gethostbyname2, (const char *name, int af), NULL)

void setnetent(int x)
{
}

struct netent *getnetent()
{
  return 0;
}

void endnetent(void)
{
}

struct netent *getnetbyaddr(uint32_t net, int type)
{
  return 0;
}

struct netent *getnetbyname(const char *name)
{
  return 0;
}

void setservent(int x)
{
}

void endservent(void)
{
}

struct servent *getservent(void)
{
  return NULL;
}

struct servent *getservbyname(const char *name __unused,
                              const char *proto __unused)
{
  return NULL;
}

struct servent *getservbyport(int port __unused,
                              const char *proto __unused)
{
  return NULL;
}

int getservbyport_r(int port __unused, const char *prots __unused,
                    struct servent *se __unused, char *buf __unused,
                    size_t buflen __unused, struct servent **res __unused)
{
  errno = ENOSYS;
  return EAI_SYSTEM;
}

static size_t idx;
static const unsigned char protos[] = {
    "\000ip\0"
    "\001icmp\0"
    "\002igmp\0"
    "\003ggp\0"
    "\004ipencap\0"
    "\005st\0"
    "\006tcp\0"
    "\010egp\0"
    "\014pup\0"
    "\021udp\0"
    "\024hmp\0"
    "\026xns-idp\0"
    "\033rdp\0"
    "\035iso-tp4\0"
    "\044xtp\0"
    "\045ddp\0"
    "\046idpr-cmtp\0"
    "\051ipv6\0"
    "\053ipv6-route\0"
    "\054ipv6-frag\0"
    "\055idrp\0"
    "\056rsvp\0"
    "\057gre\0"
    "\062esp\0"
    "\063ah\0"
    "\071skip\0"
    "\072ipv6-icmp\0"
    "\073ipv6-nonxt\0"
    "\074ipv6-opts\0"
    "\111rspf\0"
    "\121vmtp\0"
    "\131ospf\0"
    "\136ipip\0"
    "\142encap\0"
    "\147pim\0"
    "\377raw"};

void endprotoent(void)
{
  idx = 0;
}

void setprotoent(int stayopen)
{
  idx = 0;
}

struct protoent *getprotoent(void)
{
  static struct protoent p;
  static const char *aliases;

  if (idx >= sizeof(protos))
    return NULL;
  p.p_proto = protos[idx];
  p.p_name = (char *)&protos[idx + 1];
  p.p_aliases = (char **)&aliases;
  idx += strlen(p.p_name) + 2;
  return &p;
}

struct protoent *getprotobyname(const char *name)
{
  struct protoent *p;

  endprotoent();
  do
    p = getprotoent();
  while (p && strcmp(name, p->p_name));
  return p;
}

struct protoent *getprotobynumber(int num)
{
  struct protoent *p;

  endprotoent();
  do
    p = getprotoent();
  while (p && p->p_proto != num);
  return p;
}

// arpa/inet.h

uint32_t htonl(uint32_t n)
{
  union
  {
    int i;
    char c;
  } u = {1};
  return u.c ? bswap_32(n) : n;
}

uint16_t htons(uint16_t n)
{
  union
  {
    int i;
    char c;
  } u = {1};
  return u.c ? bswap_16(n) : n;
}

uint32_t ntohl(uint32_t n)
{
  union
  {
    int i;
    char c;
  } u = {1};
  return u.c ? bswap_32(n) : n;
}

uint16_t ntohs(uint16_t n)
{
  union
  {
    int i;
    char c;
  } u = {1};
  return u.c ? bswap_16(n) : n;
}

in_addr_t inet_addr(const char *p)
{
  struct in_addr a;
  if (!inet_aton(p, &a))
    return -1;
  return a.s_addr;
}

in_addr_t inet_network(const char *p)
{
  return ntohl(inet_addr(p));
}

char *inet_ntoa(struct in_addr in)
{
  static char buf[16];
  unsigned char *a = (void *)&in;

  snprintf(buf, sizeof(buf), "%d.%d.%d.%d", a[0], a[1], a[2], a[3]);

  return buf;
}

int inet_aton(const char *s0, struct in_addr *dest)
{
  const char *s = s0;
  unsigned char *d = (void *)dest;
  unsigned long a[4] = {0};
  char *z;
  int i;

  for (i = 0; i < 4; i++)
  {
    a[i] = strtoul(s, &z, 0);
    if (z == s || (*z && *z != '.') || !isdigit(*s))
      return 0;
    if (!*z)
      break;
    s = z + 1;
  }
  if (i == 4)
    return 0;
  switch (i)
  {
  case 0:
    a[1] = a[0] & 0xffffff;
    a[0] >>= 24;
  case 1:
    a[2] = a[1] & 0xffff;
    a[1] >>= 16;
  case 2:
    a[3] = a[2] & 0xff;
    a[2] >>= 8;
  }
  for (i = 0; i < 4; i++)
  {
    if (a[i] > 255)
      return 0;
    d[i] = a[i];
  }
  return 1;
}

const char *inet_ntop(int af, const void *restrict a0, char *restrict s, socklen_t l)
{
  const unsigned char *a = a0;
  int i, j, max, best;
  char buf[100];

  switch (af)
  {
  case AF_INET:
    if (snprintf(s, l, "%d.%d.%d.%d", a[0], a[1], a[2], a[3]) < l)
      return s;
    break;
  case AF_INET6:
    if (memcmp(a, "\0\0\0\0\0\0\0\0\0\0\377\377", 12))
      snprintf(buf, sizeof buf,
               "%x:%x:%x:%x:%x:%x:%x:%x",
               256 * a[0] + a[1], 256 * a[2] + a[3],
               256 * a[4] + a[5], 256 * a[6] + a[7],
               256 * a[8] + a[9], 256 * a[10] + a[11],
               256 * a[12] + a[13], 256 * a[14] + a[15]);
    else
      snprintf(buf, sizeof buf,
               "%x:%x:%x:%x:%x:%x:%d.%d.%d.%d",
               256 * a[0] + a[1], 256 * a[2] + a[3],
               256 * a[4] + a[5], 256 * a[6] + a[7],
               256 * a[8] + a[9], 256 * a[10] + a[11],
               a[12], a[13], a[14], a[15]);
    /* Replace longest /(^0|:)[:0]{2,}/ with "::" */
    for (i = best = 0, max = 2; buf[i]; i++)
    {
      if (i && buf[i] != ':')
        continue;
      j = strspn(buf + i, ":0");
      /* The leading sequence of zeros (best==0) is
       * disadvantaged compared to sequences elsewhere
       * as it doesn't have a leading colon. One extra
       * character is required for another sequence to
       * beat it fairly. */
      if (j > max + (best == 0))
        best = i, max = j;
    }
    if (max > 3)
    {
      buf[best] = buf[best + 1] = ':';
      memmove(buf + best + 2, buf + best + max, i - best - max + 1);
    }
    if (strlen(buf) < l)
    {
      strcpy(s, buf);
      return s;
    }
    break;
  default:
    errno = EAFNOSUPPORT;
    return 0;
  }
  errno = ENOSPC;
  return 0;
}

static int hexval(unsigned c)
{
  if (c - '0' < 10)
    return c - '0';
  c |= 32;
  if (c - 'a' < 6)
    return c - 'a' + 10;
  return -1;
}

int inet_pton(int af, const char *restrict s, void *restrict a0)
{
  uint16_t ip[8];
  unsigned char *a = a0;
  int i, j, v, d, brk = -1, need_v4 = 0;

  if (af == AF_INET)
  {
    for (i = 0; i < 4; i++)
    {
      for (v = j = 0; j < 3 && isdigit(s[j]); j++)
        v = 10 * v + s[j] - '0';
      if (j == 0 || (j > 1 && s[0] == '0') || v > 255)
        return 0;
      a[i] = v;
      if (s[j] == 0 && i == 3)
        return 1;
      if (s[j] != '.')
        return 0;
      s += j + 1;
    }
    return 0;
  }
  else if (af != AF_INET6)
  {
    errno = EAFNOSUPPORT;
    return -1;
  }

  if (*s == ':' && *++s != ':')
    return 0;

  for (i = 0;; i++)
  {
    if (s[0] == ':' && brk < 0)
    {
      brk = i;
      ip[i & 7] = 0;
      if (!*++s)
        break;
      if (i == 7)
        return 0;
      continue;
    }
    for (v = j = 0; j < 4 && (d = hexval(s[j])) >= 0; j++)
      v = 16 * v + d;
    if (j == 0)
      return 0;
    ip[i & 7] = v;
    if (!s[j] && (brk >= 0 || i == 7))
      break;
    if (i == 7)
      return 0;
    if (s[j] != ':')
    {
      if (s[j] != '.' || (i < 6 && brk < 0))
        return 0;
      need_v4 = 1;
      i++;
      ip[i & 7] = 0;
      break;
    }
    s += j + 1;
  }
  if (brk >= 0)
  {
    memmove(ip + brk + 7 - i, ip + brk, 2 * (i + 1 - brk));
    for (j = 0; j < 7 - i; j++)
      ip[brk + j] = 0;
  }
  for (j = 0; j < 8; j++)
  {
    *a++ = ip[j] >> 8;
    *a++ = ip[j];
  }
  if (need_v4 && inet_pton(AF_INET, (void *)s, a - 4) <= 0)
    return 0;
  return 1;
}
