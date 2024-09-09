// SPDX-FileCopyrightText: 2024 TriliTech <contact@trili.tech>
// SPDX-License-Identifier: MIT

#define _GNU_SOURCE
#include <reent.h>
#include <errno.h>
#include <unistd.h>
#include <limits.h>
#include <time.h>
#include <pthread.h>
#include <hermit.h>

long sysconf(int name)
{

  switch (name)
  {
  case _SC_ARG_MAX:
    return ARG_MAX;
  case _SC_CLK_TCK:
    return CLK_TCK;
  case _SC_NPROCESSORS_CONF:
    return sys_available_parallelism();
  case _SC_NPROCESSORS_ONLN:
    return sys_available_parallelism();
  case _SC_PAGESIZE:
    return sys_getpagesize();
  case _SC_PRIORITY_SCHEDULING:
    return 1;
  case _SC_TIMERS:
    return 1;
#if defined(_POSIX_THREADS)
  case _SC_THREAD_DESTRUCTOR_ITERATIONS:
    return PTHREAD_DESTRUCTOR_ITERATIONS;
  case _SC_THREAD_KEYS_MAX:
    return PTHREAD_KEYS_MAX;
  case _SC_THREAD_STACK_MIN:
    return PTHREAD_STACK_MIN;
  case _SC_THREAD_THREADS_MAX:
    return PTHREAD_THREADS_MAX;
  case _SC_THREADS:
    return 1;
  case _SC_THREAD_ATTR_STACKADDR:
    return -1;
  case _SC_THREAD_ATTR_STACKSIZE:
    return 1;
  case _SC_THREAD_PRIORITY_SCHEDULING:
    return -1;
  case _SC_THREAD_PRIO_INHERIT:
    return -1;
  case _SC_THREAD_PRIO_PROTECT:
    return -1;
  case _SC_THREAD_PROCESS_SHARED:
    return -1;
  case _SC_THREAD_SAFE_FUNCTIONS:
    return 1;
#endif
  case _SC_VERSION:
    return _POSIX_VERSION;
  default:
    errno = EINVAL;
    return -1;
  }
}

long fpathconf(int fd, int name)
{
  static const short values[] = {
      [_PC_LINK_MAX] = _POSIX_LINK_MAX,
      [_PC_MAX_CANON] = _POSIX_MAX_CANON,
      [_PC_MAX_INPUT] = _POSIX_MAX_INPUT,
      [_PC_NAME_MAX] = NAME_MAX,
      [_PC_PATH_MAX] = PATH_MAX,
      [_PC_PIPE_BUF] = PIPE_BUF,
      [_PC_CHOWN_RESTRICTED] = 1,
      [_PC_NO_TRUNC] = 1,
      [_PC_VDISABLE] = 0,
      [_PC_SYNC_IO] = 1,
      [_PC_ASYNC_IO] = -1,
      [_PC_PRIO_IO] = -1,
      [_PC_FILESIZEBITS] = 64,
      [_PC_REC_INCR_XFER_SIZE] = 4096,
      [_PC_REC_MAX_XFER_SIZE] = 4096,
      [_PC_REC_MIN_XFER_SIZE] = 4096,
      [_PC_REC_XFER_ALIGN] = 4096,
      [_PC_ALLOC_SIZE_MIN] = 4096,
      [_PC_SYMLINK_MAX] = -1,
      [_PC_2_SYMLINKS] = 1};
  if (name >= sizeof(values) / sizeof(values[0]))
  {
    errno = EINVAL;
    return -1;
  }
  return values[name];
}

long pathconf(const char *path, int name)
{
  return fpathconf(-1, name);
}