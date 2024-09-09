// SPDX-FileCopyrightText: 2024 TriliTech <contact@trili.tech>
// SPDX-License-Identifier: MIT

#include <sys/stat.h>
#include <reent.h>
#include <errno.h>
#include <hermit.h>

HERMIT_STUB(int, chmod, (const char *path, mode_t mode), -1)
HERMIT_STUB(int, fchmod, (int fd, mode_t mode), -1)
HERMIT_STUB(int, fchmodat, (int fd, const char *path, mode_t mode, int flag), -1)

int fstat(int fd, struct stat *st)
{
	return _fstat_r(_REENT, fd, st);
}

int _fstat_r(struct _reent *reent, int fd, struct stat *st)
{
	if (fd < 0)
	{
		reent->_errno = EBADF;
		return -1;
	}

	int ret = sys_fstat(fd, st);
	if (ret < 0)
	{
		reent->_errno = -ret;
		return -1;
	}

	return ret;
}

HERMIT_STUB(int, fstatat, (int fd, const char *path, struct stat *st, int flag), -1)

HERMIT_STUB(int, futimens, (int fd, const struct timespec times[2]), -1)
HERMIT_STUB(int, futimesat, (int dirfd, const char *pathname, const struct timeval times[2]), -1)

HERMIT_STUB(int, lchmod, (const char *path, mode_t mode), -1)
HERMIT_SYSCALL(int, lstat, (const char *path, struct stat *st), (path, st))

HERMIT_SYSCALL(int, mkdir, (const char *path, mode_t mode), (path, mode))
HERMIT_STUB(int, mkdirat, (int fd, const char *path, mode_t mode), -1)

HERMIT_STUB(int, mkfifo, (const char *path, mode_t mode), -1)
HERMIT_STUB(int, mkfifoat, (int fd, const char *path, mode_t mode), -1)

HERMIT_STUB(int, mknod, (const char *path, mode_t mode, dev_t dev), -1)
HERMIT_STUB(int, mknodat, (int fd, const char *path, mode_t mode, dev_t dev), -1)

HERMIT_SYSCALL(int, stat, (const char *path, struct stat *st), (path, st))

HERMIT_STUB(mode_t, umask, (mode_t mask), -1)

HERMIT_STUB(int, utimensat, (int fd, const char *path, const struct timespec times[2], int flag), -1)
HERMIT_STUB(int, futimes, (int fd, const struct timeval times[2]), -1)
HERMIT_STUB(int, lutimes, (const char *path, const struct timeval times[2]), -1)
