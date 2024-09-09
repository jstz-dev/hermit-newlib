// SPDX-FileCopyrightText: 2024 TriliTech <contact@trili.tech>
// SPDX-License-Identifier: MIT

#include <time.h>
#include <sys/time.h>
#include <sys/timeb.h>
#include <reent.h>
#include <errno.h>
#include <unistd.h>
#include <hermit.h>

HERMIT_STUB(int, adjtime, (const struct timeval *delta, struct timeval *olddelta), -1)

int clock_getcpuclockid(pid_t pid, clockid_t *clockid)
{
  if (pid < 0)
  {
    errno = ESRCH;
    return -1;
  }

  if (pid != 0 && pid != getpid())
  {
    errno = ENOSYS;
    return -1;
  }

  if (clockid)
  {
    *clockid = CLOCK_PROCESS_CPUTIME_ID;
  }

  return 0;
}

HERMIT_SYSCALL(int, clock_getres, (clockid_t clockid, struct timespec *ts), (clockid, ts))
HERMIT_SYSCALL(int, clock_gettime, (clockid_t clockid, struct timespec *ts), (clockid, ts))
HERMIT_SYSCALL(int, clock_nanosleep, (clockid_t clockid, int flags, const struct timespec *rqtp, struct timespec *rmtp), (clockid, flags, rqtp, rmtp))
HERMIT_STUB(int, clock_settime, (clockid_t clockid, const struct timespec *ts), -1)

int ftime(struct timeb *tp)
{
  struct timespec ts;
  clock_gettime(CLOCK_REALTIME, &ts);
  tp->time = ts.tv_sec;
  tp->millitm = ts.tv_nsec / 1000000;
  tp->timezone = tp->dstflag = 0;
  return 0;
}

HERMIT_STUB_REENT(struct tm *, getdate, (const char *string), (string), NULL)

HERMIT_SYSCALL_REENT(int, gettimeofday, (struct timeval * tv, void *tz), (tv, tz))

int settimeofday(const struct timeval *tv, const struct timezone *tz)
{
  if (!tv)
    return 0;
  if (tv->tv_usec >= 1000000ULL)
  {
    errno = EINVAL;
    return -1;
  }
  return clock_settime(CLOCK_REALTIME, &((struct timespec){
                                           .tv_sec = tv->tv_sec, .tv_nsec = tv->tv_usec * 1000}));
}

int nanosleep(const struct timespec *req, struct timespec *rem)
{
  return clock_nanosleep(CLOCK_REALTIME, 0, req, rem);
}

HERMIT_STUB_REENT(clock_t, times, (struct tms * ptms), (ptms), -1)

#define TIME_UTC 1

int timespec_get(struct timespec *ts, int base)
{
  if (base != TIME_UTC)
    return 0;
  int ret = clock_gettime(CLOCK_REALTIME, ts);
  return ret < 0 ? 0 : base;
}

HERMIT_STUB(int, utimes, (const char *path, const struct timeval times[2]), -1)
