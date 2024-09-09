// SPDX-FileCopyrightText: 2024 TriliTech <contact@trili.tech>
// SPDX-License-Identifier: MIT

#include <reent.h>
#include <errno.h>
#include <sys/mman.h>
#include <hermit.h>

// Hermit's `mmap` doesn't support mapping of files, so we only support `MAP_PRIVATE | MAP_ANONYMOUS`
void *mmap(void *addr, size_t len, int32_t prot, int32_t flags, int32_t fd, off_t offset)
{
  if (addr != NULL || flags != (MAP_PRIVATE | MAP_ANONYMOUS) || fd != -1 || offset != 0)
  {
    errno = ENOSYS;
    return NULL;
  }

  void *ret;
  int32_t status = sys_mmap(len, prot, &ret);
  if (status < 0)
  {
    errno = -status;
    return MAP_FAILED;
  }

  return ret;
}

int32_t munmap(void *addr, size_t len)
{
  int32_t status = sys_munmap(addr, len);
  if (status < 0)
  {
    errno = -status;
    return -1;
  }

  return status;
}

int32_t mprotect(void *addr, size_t len, int32_t prot)
{
  int32_t status = sys_mprotect(addr, len, prot);
  if (status < 0)
  {
    errno = -status;
    return -1;
  }

  return status;
}

HERMIT_STUB(int, msync, (void *addr, size_t len, int flags), -1)

HERMIT_STUB(int, posix_madvise, (void *addr, size_t len, int advice), -1)
HERMIT_STUB(int, mlock, (const void *addr, size_t len), -1)
HERMIT_STUB(int, munlock, (const void *addr, size_t len), -1)
HERMIT_STUB(int, mlockall, (int flags), -1)
HERMIT_STUB(int, munlockall, (void), -1)
HERMIT_STUB(int, shm_open, (const char *name, int oflag, mode_t mode), -1)
HERMIT_STUB(int, shm_unlink, (const char *name), -1)