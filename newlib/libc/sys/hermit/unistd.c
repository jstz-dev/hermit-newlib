// SPDX-FileCopyrightText: 2024 TriliTech <contact@trili.tech>
// SPDX-License-Identifier: MIT

#define __MISC_VISIBLE 1
#include <unistd.h>
#include <reent.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include <stdlib.h>
#include <hermit.h>

void _exit(int32_t status)
{
  sys_exit(status);
}

int access(const char *pathname, int mode)
{
  // Open a file descriptor, do not create a file if it doesn't exist
  int fd = open(pathname, 0);

  if (fd < 0)
  {
    return -1;
  }

  if (mode == F_OK)
    goto ok;

  struct stat st;
  if (fstat(fd, &st) < 0)
    goto err;

  int uid = getuid();
  if (uid < 0)
    goto err;

  int gid = getgid();
  if (gid < 0)
    goto err;

  // Last 3 octets of `st.st_mode` are the permissions
  // The first (left) is user, the second is group, the last is others
  int permidx = st.st_uid == uid ? 2 : st.st_gid == gid ? 1
                                                        : 0;
  int perms = st.st_mode >> (permidx * 3) & 07;

  if ((mode & R_OK == R_OK && perms & R_OK != R_OK) || (mode & W_OK == W_OK && perms & W_OK != W_OK) || (mode & X_OK == X_OK && perms & X_OK != X_OK))
  {
    errno = EINVAL;
    goto err;
  }

  goto ok;

err:
  close(fd);
  return -1;

ok:
  close(fd);
  return 0;
}

// Process accounting is disabled for hermit
HERMIT_STUB(int, acct, (const char *file), -1);

unsigned int alarm(unsigned int seconds)
{
  // Hermit doesn't support alarms
  // If no alarm is currently set, the function returns 0
  return 0;
}

HERMIT_STUB(int, chdir, (const char *path), -1)

HERMIT_STUB(int, chown, (const char *path, uid_t owner, gid_t group), -1)

HERMIT_SYSCALL_REENT(int, close, (int fd), (fd))

char *ctermid(char *s)
{
  // Hermit doesn't support controlling terminals
  return NULL;
}

HERMIT_SYSCALL(int, dup, (int fd), (fd))
HERMIT_STUB(int, dup2, (int oldfd, int newfd), -1)
HERMIT_STUB(int, dup3, (int oldfd, int newfd, int flags), -1)

HERMIT_STUB(int, faccessat, (int dirfd, const char *path, int mode, int flags), -1)
HERMIT_STUB(int, fchdir, (int fd), -1)
HERMIT_STUB(int, fchown, (int fd, uid_t owner, gid_t group), -1)
HERMIT_STUB(int, fchownat, (int dirfd, const char *path, uid_t owner, gid_t group, int flags), -1)
HERMIT_STUB(int, fdatasync, (int fd), -1)
HERMIT_STUB(int, fsync, (int fd), -1)
HERMIT_STUB(int, ftruncate, (int fd, off_t length), -1)

// TODO:
// We could emulate this by having a global variable that stores the current working directory
// and then return that value when getcwd is called

HERMIT_STUB(char *, getcwd, (char *buf, size_t size), NULL)
HERMIT_STUB(char *, getwd, (char *buf), NULL)

gid_t getegid(void)
{
  // See getgid
  return HERMIT_GID;
}

uid_t geteuid(void)
{
  // See getuid
  return HERMIT_UID;
}

gid_t getgid(void)
{
  return HERMIT_GID;
}

int getgroups(int gidsetsize, gid_t grouplist[])
{
  // If the gidsetsize is 0, we return number of
  // *supplementary* groups of the process
  if (gidsetsize == 0)
  {
    return 0;
  }

  if (grouplist == NULL)
  {
    errno = EFAULT;
    return -1;
  }

  // assert(gidsetsize > 0 || gidsetsize < 0)

  // EINVAL if `gidsetsize is less than the number of supplementary group IDs,
  // but is not zero.
  errno = EINVAL;
  return -1;
}

char *getlogin(void)
{
  // In hermit, we don't have the concept of login
  // So, we return the username of the user (which will be root)
  return getenv("USER");
}

pid_t getpgid(pid_t pid)
{
  if (pid == 0)
  {
    return HERMIT_PGID;
  }

  if (pid != 1)
  {
    errno = ESRCH;
    return -1;
  }

  return HERMIT_PGID;
}

pid_t getpgrp(void)
{
  return getpgid(0);
}

int _getpid_r(struct _reent *reent)
{
  return HERMIT_INIT_PID;
}

int getpid(void)
{
  return _getpid_r(_REENT);
}

pid_t getppid(void)
{
  return HERMIT_INIT_PPID;
}

uid_t getuid(void)
{
  return HERMIT_UID;
}

// This function is to be kept in sync with the implementation from hermit-abi/src/lib.rs

int _isatty_r(struct _reent *reent, int file)
{
  return 0;
}

int isatty(int file)
{
  return _isatty_r(_REENT, file);
}

HERMIT_STUB(int, lchown, (const char *path, uid_t uid, gid_t gid), -1)
HERMIT_STUB_REENT(int, link, (const char *path1, const char *path2), (path1, path2), -1)
HERMIT_STUB(int, linkat, (int fd, const char *existing, int fd2, const char *new, int flag), -1)

HERMIT_SYSCALL_REENT(off_t, lseek, (int fd, off_t offset, int whence), (fd, offset, whence))

HERMIT_STUB(int, nice, (int inc), -1)
HERMIT_STUB(int, pause, (void), -1)

HERMIT_STUB(int, pipe, (int fd[2]), -1)
HERMIT_STUB(int, pipe2, (int fd[2], int flag), -1)

int posix_close(int fd, int flags)
{
  return close(fd);
}

HERMIT_STUB(ssize_t, pread, (int fd, void *buf, size_t count, off_t offset), -1)
HERMIT_STUB(ssize_t, preadv, (int fd, const struct iovec *iov, int iovcnt, off_t offset), -1)

HERMIT_SYSCALL_REENT(ssize_t, read, (int fd, void *buf, size_t len), (fd, buf, len))

HERMIT_STUB(ssize_t, readlink, (const char *path, char *buf, size_t len), -1)
HERMIT_STUB(ssize_t, readlinkat, (int fd, const char *path, char *buf, size_t len), -1)

HERMIT_SYSCALL(ssize_t, readv, (int fd, const struct iovec *iov, int count), (fd, iov, count))

HERMIT_STUB(int, renameat, (int fromfd, const char *from, int tofd, const char *to), -1)

HERMIT_SYSCALL(int, rmdir, (const char *path), (path))

HERMIT_STUB_WITH_ERRNO(int, setegid, (gid_t egid), EPERM, -1)
HERMIT_STUB_WITH_ERRNO(int, seteuid, (uid_t euid), EPERM, -1)
HERMIT_STUB_WITH_ERRNO(int, setgid, (gid_t gid), EPERM, -1)
HERMIT_STUB_WITH_ERRNO(int, setpgid, (pid_t pid, pid_t pgid), EPERM, -1)
HERMIT_STUB_WITH_ERRNO(int, setpgrp, (void), EPERM, -1)
HERMIT_STUB_WITH_ERRNO(int, setregid, (gid_t rgid, gid_t egid), EPERM, -1)
HERMIT_STUB_WITH_ERRNO(int, setresgid, (gid_t rgid, gid_t egid, gid_t sgid), EPERM, -1)
HERMIT_STUB_WITH_ERRNO(int, setresuid, (uid_t ruid, uid_t euid, uid_t suid), EPERM, -1)
HERMIT_STUB_WITH_ERRNO(int, setreuid, (uid_t ruid, uid_t euid), EPERM, -1)
HERMIT_STUB_WITH_ERRNO(pid_t, setsid, (void), EPERM, -1)
HERMIT_STUB_WITH_ERRNO(int, setuid, (uid_t uid), EPERM, -1)

unsigned sleep(unsigned seconds)
{
  struct timespec tv = {.tv_sec = seconds, .tv_nsec = 0};
  if (nanosleep(&tv, &tv))
    return tv.tv_sec;
  return 0;
}

HERMIT_STUB(int, symlink, (const char *existing, const char *new), -1)
HERMIT_STUB(int, symlinkat, (const char *existing, int fd, const char *new), -1)

void sync(void)
{
  // Hermit either uses a virtiofs or a memfs, either of which doesn't require syncing
}

HERMIT_STUB(int, truncate, (const char *path, off_t length), -1)

HERMIT_STUB(int, ttyname_r, (int fd, char *buf, size_t buflen), -1)
HERMIT_STUB(char *, ttyname, (int fd), NULL)

useconds_t ualarm(useconds_t usecs, useconds_t interval)
{
  // Hermit doesn't support alarms (see alarm(1))
  return 0;
}

HERMIT_SYSCALL_REENT(int, unlink, (const char *path), (path))

HERMIT_STUB(int, unlinkat, (int fd, const char *path, int flag), -1)

int usleep(useconds_t useconds)
{
  struct timespec tv =
      {
          .tv_sec = useconds / 1000000,
          .tv_nsec = (useconds % 1000000) * 1000};

  return nanosleep(&tv, &tv);
}

HERMIT_SYSCALL_REENT(ssize_t, write, (int fd, const void *buf, size_t count), (fd, buf, count))
HERMIT_SYSCALL(ssize_t, writev, (int fd, const struct iovec *iov, int iovcnt), (fd, iov, iovcnt))

int lockf(int fd, int op, off_t size)
{
  struct flock l = {
      .l_type = F_WRLCK,
      .l_whence = SEEK_CUR,
      .l_len = size,
  };
  switch (op)
  {
  case F_TEST:
    l.l_type = F_RDLCK;
    if (fcntl(fd, F_GETLK, &l) < 0)
      return -1;
    if (l.l_type == F_UNLCK || l.l_pid == getpid())
      return 0;
    errno = EACCES;
    return -1;
  case F_ULOCK:
    l.l_type = F_UNLCK;
  case F_TLOCK:
    return fcntl(fd, F_SETLK, &l);
  case F_LOCK:
    return fcntl(fd, F_SETLKW, &l);
  }
  errno = EINVAL;
  return -1;
}