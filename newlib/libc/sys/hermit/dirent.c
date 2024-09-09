// SPDX-FileCopyrightText: 2024 TriliTech <contact@trili.tech>
// SPDX-License-Identifier: MIT

#include <dirent.h>
#include <limits.h>
#include <reent.h>
#include <errno.h>
#include <hermit.h>

int getdents(int fd, struct dirent *dp, size_t len)
{
  if (len > SIZE_MAX)
    len = SIZE_MAX;
  int ret = sys_getdents64(fd, dp, len);

  if (ret < 0)
  {
    errno = -ret;
    return -1;
  }

  return ret;
}