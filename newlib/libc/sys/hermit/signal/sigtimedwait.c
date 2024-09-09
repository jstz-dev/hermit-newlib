// SPDX-FileCopyrightText: 2024 TriliTech <contact@trili.tech>
// SPDX-License-Identifier: MIT

#include <signal.h>
#include <errno.h>

extern sigset_t __sig_pending;

int sigtimedwait(const sigset_t *restrict mask, siginfo_t *restrict si, const struct timespec *restrict timeout)
{
  for (int sig = 0; sig < NSIG; sig++)
  {
    if (sigismember(mask, sig) && sigismember(&__sig_pending, sig))
    {
      if (si)
      {
        siginfo_t t = {0};
        *si = t;
      }
      sigdelset(&__sig_pending, sig);
      return sig;
    }
  }

  errno = EINVAL;
  return -1;
}