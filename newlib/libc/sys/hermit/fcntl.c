// SPDX-FileCopyrightText: 2024 TriliTech <contact@trili.tech>
// SPDX-License-Identifier: MIT

#include <fcntl.h>
#include <stdarg.h>
#include <reent.h>
#include <errno.h>
#include <hermit.h>

int creat(const char *filename, mode_t mode)
{
  return open(filename, O_WRONLY | O_CREAT | O_TRUNC, mode);
}

int fcntl(int fd, int cmd, ...)
{
  unsigned long arg;
  va_list ap;
  arg = va_arg(ap, unsigned long);
  va_end(ap);

  int ret = sys_fcntl(fd, cmd, arg);
  if (ret < 0)
  {
    errno = -ret;
    return -1;
  }

  return ret;
}

int open(const char *filename, int flags, ...)
{
  int mode = 0;
  if (flags & O_CREAT)
  {
    va_list ap;
    va_start(ap, flags);
    mode = va_arg(ap, int);
    va_end(ap);
  }

  return _open_r(_REENT, filename, flags, mode);
}

int _open_r(struct _reent *reent, const char *filename, int flags, int mode)
{
  int fd = sys_open(filename, flags, mode);
  if (fd < 0)
  {
    reent->_errno = -fd;
    return -1;
  }

  return fd;
}

int openat(int fd, const char *filename, int flags, ...)
{
  errno = ENOSYS;
  return -1;
}
