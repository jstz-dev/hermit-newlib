// SPDX-FileCopyrightText: 2024 TriliTech <contact@trili.tech>
// SPDX-License-Identifier: MIT

#ifndef _HERMIT_ABI_H
#define _HERMIT_ABI_H

#include <stddef.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>

#include <sys/types.h>    // for `ssize_t`, `pid_t, `clock_t`
#include <sys/timespec.h> // for `time_t`, `timespec`
#include <hermit/stat.h>  // for `stat`
#include <sys/dirent.h>   // for `dirent`
#include <sys/uio.h>      // for `iovec`
#include <poll.h>     // for `pollfd`
#include <netdb.h>        // for `addrinfo`

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/**
 * Maximum number of priorities
 */
#define NO_PRIORITIES 31

#define FUTEX_RELATIVE_TIMEOUT 1

// FIXME:
// Some of these flags don't match those in _default_fcntl.h
#define O_RDONLY 0

#define O_WRONLY 1

#define O_RDWR 2

#define O_CREAT 64

#define O_EXCL 128

#define O_TRUNC 512

#define O_APPEND 1024

#define O_NONBLOCK 2048

#define O_DIRECTORY 65536

#define F_DUPFD 0

#define F_GETFD 1

#define F_SETFD 2

#define F_GETFL 3

#define F_SETFL 4

#define FD_CLOEXEC 1f

#define AF_INET 0

#define AF_INET6 1

#define IPPROTO_IP 0

#define IPPROTO_IPV6 41

#define IPPROTO_UDP 17

#define IPPROTO_TCP 6

#define IPV6_ADD_MEMBERSHIP 12

#define IPV6_DROP_MEMBERSHIP 13

#define IPV6_MULTICAST_LOOP 19

#define IPV6_V6ONLY 27

#define IP_TOS 1

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

#define SOCK_NONBLOCK 2048

#define SOCK_CLOEXEC 16384

#define SOL_SOCKET 4095

#define SO_REUSEADDR 4

#define SO_KEEPALIVE 8

#define SO_BROADCAST 32

#define SO_LINGER 128

#define SO_SNDBUF 4097

#define SO_RCVBUF 4098

#define SO_SNDTIMEO 4101

#define SO_RCVTIMEO 4102

#define SO_ERROR 4103

#define TCP_NODELAY 1

#define MSG_PEEK 1

#define FIONBIO (int32_t)2148034174u

#define EFD_SEMAPHORE 1

#define EFD_NONBLOCK 2048

#define EFD_CLOEXEC 16384

#define IOV_MAX 1024

#define S_IFIFO 4096

#define S_IFCHR 8192

#define S_IFBLK 24576

#define S_IFDIR 16384

#define S_IFREG 32768

#define S_IFLNK 40960

#define S_IFSOCK 49152

#define S_IFMT 61440

/**
 * Pages may not be accessed.
 */
#define PROT_NONE 0

/**
 * Indicates that the memory region should be readable.
 */
#define PROT_READ (1 << 0)

/**
 * Indicates that the memory region should be writable.
 */
#define PROT_WRITE (1 << 1)

/**
 * Indicates that the memory region should be executable.
 */
#define PROT_EXEC (1 << 2)

/**
 * The file offset is set to offset bytes.
 */
#define SEEK_SET 0

/**
 * The file offset is set to its current location plus offset bytes.
 */
#define SEEK_CUR 1

/**
 * The file offset is set to the size of the file plus offset bytes.
 */
#define SEEK_END 2

  typedef const void *sem_t;

  /**
   * A thread handle type
   */
  typedef uint32_t tid_t;

#define CLOCK_REALTIME 1

#define CLOCK_PROCESS_CPUTIME_ID 2

#define CLOCK_THREAD_CPUTIME_ID 3

#define CLOCK_MONOTONIC 4

  /**
   * Get the last error number from the thread local storage
   */
  extern int32_t sys_get_errno(void);

  /**
   * Get the last error number from the thread local storage
   */
  extern int32_t sys_errno(void);

  /**
   * Get memory page size
   */
  extern int32_t sys_getpagesize(void);

  /**
   * Creates a new virtual memory mapping of the `size` specified with
   * protection bits specified in `prot_flags`.
   */
  extern int32_t sys_mmap(size_t size, uint32_t prot_flags, void **ret);

  /**
   * Unmaps memory at the specified `ptr` for `size` bytes.
   */
  extern int32_t sys_munmap(void *ptr, size_t size);

  /**
   * Configures the protections associated with a region of virtual memory
   * starting at `ptr` and going to `size`.
   *
   * Returns 0 on success and an error code on failure.
   */
  extern int32_t sys_mprotect(void *ptr, size_t size, uint32_t prot_flags);

  /**
   * If the value at address matches the expected value, park the current thread until it is either
   * woken up with [`futex_wake`] (returns 0) or an optional timeout elapses (returns -ETIMEDOUT).
   *
   * Setting `timeout` to null means the function will only return if [`futex_wake`] is called.
   * Otherwise, `timeout` is interpreted as an absolute time measured with [`CLOCK_MONOTONIC`].
   * If [`FUTEX_RELATIVE_TIMEOUT`] is set in `flags` the timeout is understood to be relative
   * to the current time.
   *
   * Returns -EINVAL if `address` is null, the timeout is negative or `flags` contains unknown values.
   */
  extern int32_t sys_futex_wait(uint32_t *address,
                                uint32_t expected,
                                const struct timespec *timeout,
                                uint32_t flags);

  /**
   * Wake `count` threads waiting on the futex at `address`. Returns the number of threads
   * woken up (saturates to `i32::MAX`). If `count` is `i32::MAX`, wake up all matching
   * waiting threads. If `count` is negative or `address` is null, returns -EINVAL.
   */
  extern int32_t sys_futex_wake(uint32_t *address, int32_t count);

  /**
   * sem_init() initializes the unnamed semaphore at the address
   * pointed to by `sem`.  The `value` argument specifies the
   * initial value for the semaphore. If `pshared` is nonzero,
   * then the semaphore is shared between processes (currently
   * not supported).
   */
  extern int32_t sys_sem_init(sem_t *sem, int32_t pshared, uint32_t value);

  /**
   * sem_destroy() frees the unnamed semaphore at the address
   * pointed to by `sem`.
   */
  extern int32_t sys_sem_destroy(sem_t *sem);

  /**
   * sem_post() increments the semaphore pointed to by `sem`.
   * If the semaphore's value consequently becomes greater
   * than zero, then another thread blocked in a sem_wait call
   * will be woken up and proceed to lock the semaphore.
   */
  extern int32_t sys_sem_post(sem_t *sem);

  /**
   * try to decrement a semaphore
   *
   * sem_trywait() is the same as sem_timedwait(), except that
   * if the  decrement cannot be immediately performed, then  call
   * returns a negative value instead of blocking.
   */
  extern int32_t sys_sem_trywait(sem_t *sem);

  /**
   * decrement a semaphore
   *
   * sem_timedwait() decrements the semaphore pointed to by `sem`.
   * If the semaphore's value is greater than zero, then the
   * the function returns immediately. If the semaphore currently
   * has the value zero, then the call blocks until either
   * it becomes possible to perform the decrement of the time limit
   * to wait for the semaphore is expired. A time limit `ms` of
   * means infinity waiting time.
   */
  extern int32_t sys_sem_timedwait(sem_t *sem, const struct timespec *abs_timeout);

  /**
   * Determines the id of the current thread
   */
  extern pid_t sys_getpid(void);

  /**
   * cause normal termination and return `status`
   * to the host system
   */
  extern void sys_exit(int32_t status) _ATTRIBUTE((__noreturn__));

  /**
   * cause abnormal termination
   */
  extern void sys_abort(void) _ATTRIBUTE((__noreturn__));

  /**
   * suspend execution for microsecond intervals
   *
   * The usleep() function suspends execution of the calling
   * thread for (at least) `usecs` microseconds.
   */
  extern void sys_usleep(uint64_t usecs);

  /**
   * suspend thread execution for an interval measured in nanoseconds
   */
  extern int32_t sys_nanosleep(const struct timespec *req);

  /**
   * spawn a new thread
   *
   * spawn() starts a new thread. The new thread starts execution
   * by invoking `func(usize)`; `arg` is passed as the argument
   * to `func`. `prio` defines the priority of the new thread,
   * which can be between `LOW_PRIO` and `HIGH_PRIO`.
   * `core_id` defines the core, where the thread is located.
   * A negative value give the operating system the possibility
   * to select the core by its own.
   */
  extern int32_t sys_spawn(tid_t *id,
                           void (*func)(void *),
                           void *arg,
                           uint8_t prio,
                           ssize_t core_id);

  /**
   * spawn a new thread with user-specified stack size
   *
   * spawn2() starts a new thread. The new thread starts execution
   * by invoking `func(usize)`; `arg` is passed as the argument
   * to `func`. `prio` defines the priority of the new thread,
   * which can be between `LOW_PRIO` and `HIGH_PRIO`.
   * `core_id` defines the core, where the thread is located.
   * A negative value give the operating system the possibility
   * to select the core by its own.
   * In contrast to spawn(), spawn2() is able to define the
   * stack size.
   */
  extern tid_t sys_spawn2(void (*func)(void *),
                          void *arg,
                          uint8_t prio,
                          size_t stack_size,
                          ssize_t core_id);

  /**
   * join with a terminated thread
   *
   * The join() function waits for the thread specified by `id`
   * to terminate.
   */
  extern int32_t sys_join(tid_t id);

  /**
   * yield the processor
   *
   * causes the calling thread to relinquish the CPU. The thread
   * is moved to the end of the queue for its static priority.
   */
  extern void sys_yield(void);

  /**
   * Finds the resolution (or precision) of a clock.
   *
   * This function gets the clock resolution of a clock with `clockid` and stores it in the parameter `tp`.
   * Returns `0` on success, `-EINVAL` otherwise.
   *
   * Supported clocks:
   * - `CLOCK_REALTIME`
   * - `CLOCK_MONOTONIC`
   * - `CLOCK_PROCESS_CPUTIME_ID`
   * - `CLOCK_THREAD_CPUTIME_ID`
   */
  extern int32_t sys_clock_getres(clockid_t clockid, struct timespec *tp);

  /**
   * get current time
   *
   * The clock_gettime() functions allow the calling thread
   * to retrieve the value used by a clock which is specified
   * by `clockid`.
   *
   * `CLOCK_REALTIME`: the system's real time clock,
   * expressed as the amount of time since the Epoch.
   *
   * `CLOCK_MONOTONIC`: clock that increments monotonically,
   * tracking the time since an arbitrary point
   */
  extern int32_t sys_clock_gettime(clockid_t clockid, struct timespec *tp);

  /**
   * Sleep a clock for a specified number of nanoseconds
   *
   * The requested time (in nanoseconds) must be greater than 0 and less than 1,000,000.
   *
   * Returns `0` on success, `-EINVAL` otherwise.
   *
   * Supported clocks:
   * - `CLOCK_REALTIME`
   * - `CLOCK_MONOTONIC`
   */
  extern int32_t sys_clock_nanosleep(clockid_t clockid, int flags, const struct timespec *rqtp, struct timespec *rmtp);

  /**
   * Set the current time
   *
   * This function is currently not supported, and always returns `-EINVAL`.
   */
  extern int32_t sys_clock_settime(clockid_t clockid, const struct timespec *tp);

  /**
   * Get the system's clock time.
   *
   * This function gets the current time based on the wallclock time when booted up, plus current timer ticks.
   * Returns `0` on success, `-EINVAL` otherwise.
   *
   * Note: `tz` should be set to `NULL` since Hermit does not support timezones.
   */
  extern int32_t sys_gettimeofday(struct timeval *tp, void *tz);

  /**
   * open and possibly create a file
   *
   * The open() system call opens the file specified by `name`.
   * If the specified file does not exist, it may optionally
   * be created by open().
   */
  extern int32_t sys_open(const char *name, int32_t flags, int32_t mode);

  /**
   * open a directory
   *
   * The opendir() system call opens the directory specified by `name`.
   */
  extern int32_t sys_opendir(const char *name);

  /**
   * delete the file it refers to `name`
   */
  extern int32_t sys_unlink(const char *name);

  /**
   * remove directory it refers to `name`
   */
  extern int32_t sys_rmdir(const char *name);

  /**
   * stat
   */
  extern int32_t sys_stat(const char *name, struct stat *stat);

  /**
   * lstat
   */
  extern int32_t sys_lstat(const char *name, struct stat *stat);

  /**
   * fstat
   */
  extern int32_t sys_fstat(int32_t fd, struct stat *stat);

  /**
   * Returns an estimate of the default amount of parallelism
   * a program should use. This number often corresponds to the
   * amount of CPUs a computer has, but it may diverge in
   * various cases.
   */
  extern size_t sys_available_parallelism(void);

  extern void *sys_malloc(size_t size, size_t align);

  extern void *sys_alloc(size_t size, size_t align);

  extern void *sys_alloc_zeroed(size_t size, size_t align);

  extern void *sys_realloc(void *ptr, size_t size, size_t align, size_t new_size);

  extern void sys_free(void *ptr, size_t size, size_t align);

  extern void sys_dealloc(void *ptr, size_t size, size_t align);

  extern int32_t sys_notify(uintptr_t id, int32_t count);

  extern int32_t sys_add_queue(uintptr_t id, int64_t timeout_ns);

  extern int32_t sys_wait(uintptr_t id);

  extern int32_t sys_init_queue(uintptr_t id);

  extern int32_t sys_destroy_queue(uintptr_t id);

  /**
   * initialize the network stack
   */
  extern int32_t sys_network_init(void);

  /**
   * Add current task to the queue of blocked tasks. After calling `block_current_task`,
   * call `yield_now` to switch to another task.
   */
  extern void sys_block_current_task(void);

  /**
   * Add current task to the queue of blocked tasks, but wake it when `timeout` milliseconds
   * have elapsed.
   *
   * After calling `block_current_task`, call `yield_now` to switch to another task.
   */
  extern void sys_block_current_task_with_timeout(uint64_t timeout);

  /**
   * Wakeup task with the thread id `tid`
   *
   * FIXME: This function has a spelling mistake in the linkage attribute
   * in lib.rs. It should be `sys_wakeup_task`, not `sys_wakeup_taskt`.
   */
  extern void sys_wakeup_taskt(tid_t tid);

  /**
   * The system call `getaddrbyname` determine the network host entry.
   * It expects an array of u8 with a size of in_addr or of in6_addr.
   * The result of the DNS request will be stored in this array.
   */
  extern int32_t sys_getaddrbyname(const char *name, uint8_t *inaddr, size_t len);

  extern int32_t sys_accept(int32_t s, struct sockaddr *addr, socklen_t *addrlen);

  /**
   * bind a name to a socket
   */
  extern int32_t sys_bind(int32_t s, const struct sockaddr *name, socklen_t namelen);

  extern int32_t sys_connect(int32_t s, const struct sockaddr *name, socklen_t namelen);

  /**
   * read from a file descriptor
   *
   * read() attempts to read `len` bytes of data from the object
   * referenced by the descriptor `fd` into the buffer pointed
   * to by `buf`.
   */
  extern ssize_t sys_read(int32_t fd, uint8_t *buf, size_t len);

  /**
   * `read()` attempts to read `nbyte` of data to the object referenced by the
   * descriptor `fd` from a buffer. `read()` performs the same
   * action, but scatters the input data from the `iovcnt` buffers specified by the
   * members of the iov array: `iov[0], iov[1], ..., iov[iovcnt-1]`.
   *
   * Each `iovec` entry specifies the base address and length of an area in memory from
   * which data should be written.  `readv()` will always fill an completely
   * before proceeding to the next.
   */
  extern ssize_t sys_readv(int32_t fd, const struct iovec *iov, size_t iovcnt);

  /**
   * `getdents64` reads directory entries from the directory referenced
   * by the file descriptor `fd` into the buffer pointed to by `buf`.
   */
  extern int64_t sys_getdents64(int32_t fd, struct dirent *dirp, size_t count);

  /**
   * 'mkdir' attempts to create a directory,
   * it returns 0 on success and -1 on error
   */
  extern int32_t sys_mkdir(const int8_t *name, uint32_t mode);

  /**
   * Fill `len` bytes in `buf` with cryptographically secure random data.
   *
   * Returns either the number of bytes written to buf (a positive value) or
   * * `-EINVAL` if `flags` contains unknown flags.
   * * `-ENOSYS` if the system does not support random data generation.
   */
  extern ssize_t sys_read_entropy(uint8_t *buf, size_t len, uint32_t flags);

  /**
   * receive() a message from a socket
   */
  extern ssize_t sys_recv(int32_t socket, uint8_t *buf, size_t len, int32_t flags);

  /**
   * receive() a message from a socket
   */
  extern ssize_t sys_recvfrom(int32_t socket,
                              uint8_t *buf,
                              size_t len,
                              int32_t flags,
                              struct sockaddr *addr,
                              socklen_t *addrlen);

  /**
   * The fseek() function sets the file position indicator for the stream pointed to by stream.
   * The new position, measured in bytes, is obtained by adding offset bytes to the position
   * specified by whence.  If whence is set to SEEK_SET, SEEK_CUR, or SEEK_END, the offset is
   * relative to the start of the file, the current position indicator, or end-of-file,
   * respectively.
   */
  extern ssize_t sys_lseek(int32_t fd, intptr_t offset, int32_t whence);

  /**
   * write to a file descriptor
   *
   * write() attempts to write `len` of data to the object
   * referenced by the descriptor `fd` from the
   * buffer pointed to by `buf`.
   */
  extern ssize_t sys_write(int32_t fd, const uint8_t *buf, size_t len);

  /**
   * `write()` attempts to write `nbyte` of data to the object referenced by the
   * descriptor `fd` from a buffer. `writev()` performs the same
   * action, but gathers the output data from the `iovcnt` buffers specified by the
   * members of the iov array: `iov[0], iov[1], ..., iov[iovcnt-1]`.
   *
   * Each `iovec` entry specifies the base address and length of an area in memory from
   * which data should be written.  `writev()` will always write a
   * complete area before proceeding to the next.
   */
  extern ssize_t sys_writev(int32_t fd, const struct iovec *iov, size_t iovcnt);

  /**
   * close a file descriptor
   *
   * The close() call deletes a file descriptor `fd` from the object
   * reference table.
   */
  extern int32_t sys_close(int32_t fd);

  /**
   * duplicate an existing file descriptor
   */
  extern int32_t sys_dup(int32_t fd);

  extern int32_t sys_getpeername(int32_t s, struct sockaddr *name, socklen_t *namelen);

  extern int32_t sys_getsockname(int32_t s, struct sockaddr *name, socklen_t *namelen);

  extern int32_t sys_getsockopt(int32_t s,
                                int32_t level,
                                int32_t optname,
                                void *optval,
                                socklen_t *optlen);

  extern int32_t sys_setsockopt(int32_t s,
                                int32_t level,
                                int32_t optname,
                                const void *optval,
                                socklen_t optlen);

  extern int32_t sys_ioctl(int32_t s, int32_t cmd, void *argp);

  extern int32_t sys_fcntl(int32_t fd, int32_t cmd, int32_t arg);

  /**
   * `eventfd` creates an linux-like "eventfd object" that can be used
   * as an event wait/notify mechanism by user-space applications, and by
   * the kernel to notify user-space applications of events. The
   * object contains an unsigned 64-bit integer counter
   * that is maintained by the kernel. This counter is initialized
   * with the value specified in the argument `initval`.
   *
   * As its return value, `eventfd` returns a new file descriptor that
   * can be used to refer to the eventfd object.
   *
   * The following values may be bitwise set in flags to change the
   * behavior of `eventfd`:
   *
   * `EFD_NONBLOCK`: Set the file descriptor in non-blocking mode
   * `EFD_SEMAPHORE`: Provide semaphore-like semantics for reads
   * from the new file descriptor.
   */
  extern int32_t sys_eventfd(uint64_t initval, int16_t flags);

  /**
   * The unix-like `poll` waits for one of a set of file descriptors
   * to become ready to perform I/O. The set of file descriptors to be
   * monitored is specified in the `fds` argument, which is an array
   * of structures of `pollfd`.
   */
  extern int32_t sys_poll(struct pollfd *fds, nfds_t nfds, int32_t timeout);

  /**
   * listen for connections on a socket
   *
   * The `backlog` parameter defines the maximum length for the queue of pending
   * connections. Currently, the `backlog` must be one.
   */
  extern int32_t sys_listen(int32_t s, int32_t backlog);

  extern ssize_t sys_send(int32_t s, const void *mem, size_t len, int32_t flags);

  extern ssize_t sys_sendto(int32_t s,
                            const void *mem,
                            size_t len,
                            int32_t flags,
                            const struct sockaddr *to,
                            socklen_t tolen);

  /**
   * shut down part of a full-duplex connection
   */
  extern int32_t sys_shutdown(int32_t sockfd, int32_t how);

  extern int32_t sys_shutdown_socket(int32_t s, int32_t how);

  extern int32_t sys_socket(int32_t domain, int32_t type_, int32_t protocol);

  extern void sys_freeaddrinfo(struct addrinfo *ai);

  extern int32_t sys_getaddrinfo(const char *nodename,
                                 const char *servname,
                                 const struct addrinfo *hints,
                                 struct addrinfo **res);

  extern uint8_t sys_get_priority(void);

  extern void sys_set_priority(tid_t tid, uint8_t prio);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif // _HERMIT_ABI_H
