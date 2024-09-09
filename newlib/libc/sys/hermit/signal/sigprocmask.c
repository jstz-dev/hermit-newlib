// SPDX-FileCopyrightText: 2024 TriliTech <contact@trili.tech>
// SPDX-License-Identifier: MIT

#define _GNU_SOURCE // for sigorset/sigandset
#include <stdbool.h>
#include <signal.h>
#include <reent.h>
#include <errno.h>

#define siginvertset(dst, src) (*(dst) = ~(*(src)), 0)

static sigset_t __sig_mask;
sigset_t __sig_pending;

bool __sig_is_blocked(int sig)
{
  return sigismember(&__sig_mask, sig);
}

int sigprocmask(int how, const sigset_t *restrict set, sigset_t *restrict old)
{
  if (old)
  {
    *old = __sig_mask;
  }

  switch (how)
  {
  case SIG_SETMASK:
    __sig_mask = *set;
    break;
  case SIG_BLOCK:
    sigorset(&__sig_mask, &__sig_mask, set);
    break;
  case SIG_UNBLOCK:
  {
    sigset_t tmp;
    siginvertset(&tmp, set);
    sigandset(&__sig_mask, &__sig_mask, &tmp);
    break;
  }
  default:
    return EINVAL;
  }

  // These two signals can never be blocked.
  sigdelset(&__sig_mask, SIGKILL);
  sigdelset(&__sig_mask, SIGSTOP);

  // Raise any pending signals that are now unblocked.
  for (int sig = 0; sig < NSIG; sig++)
  {
    if (sigismember(&__sig_pending, sig) && !sigismember(&__sig_mask, sig))
    {
      sigdelset(&__sig_pending, sig);
      raise(sig);
    }
  }

  return 0;
}

int sigpending(sigset_t *set)
{
  *set = __sig_pending;
  return 0;
}