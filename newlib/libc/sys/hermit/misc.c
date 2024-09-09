// SPDX-FileCopyrightText: 2024 TriliTech <contact@trili.tech>
// SPDX-License-Identifier: MIT

#include <sys/types.h>
#include <sys/eventfd.h>
#include <stdarg.h>
#include <reent.h>
#include <errno.h>
#include <hermit.h>

int getpagesize(void)
{
  return sys_getpagesize();
}

int ioctl(int fd, int cmd, ...)
{
  void *arg;
  va_list ap;
  va_start(ap, cmd);
  arg = va_arg(ap, void *);
  va_end(ap);

  int ret = sys_ioctl(fd, cmd, arg);
  if (ret < 0)
  {
    errno = -ret;
    return -1;
  }

  return ret;
}

HERMIT_SYSCALL(ssize_t, getrandom, (void *buf, size_t size, unsigned flags), (buf, size, flags))

int getentropy(void *buf, size_t size)
{
  int ret = 0;
  char *pos = buf;

  if (size > 256)
  {
    errno = EIO;
    return -1;
  }

  while (size)
  {
    ret = getrandom(pos, size, 0);
    if (ret < 0)
      break;
    pos += ret;
    size -= ret;
    ret = 0;
  }

  return ret;
}

HERMIT_SYSCALL(int, eventfd, (unsigned int initval, int flags), (initval, flags))
int eventfd_read(int fd, eventfd_t *value)
{
  return (sizeof(*value) == read(fd, value, sizeof(*value))) ? 0 : -1;
}

int eventfd_write(int fd, eventfd_t value)
{
  return (sizeof(value) == write(fd, &value, sizeof(value))) ? 0 : -1;
}

HERMIT_STUB(int, uname, (struct utsname * uts), -1)