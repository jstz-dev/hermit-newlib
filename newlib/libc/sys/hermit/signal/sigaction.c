// SPDX-FileCopyrightText: 2024 TriliTech <contact@trili.tech>
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <signal.h>
#include <errno.h>

struct sigaction __sig_actions[NSIG];

int sigaction(int sig, const struct sigaction *restrict sa, struct sigaction *restrict old)
{
  if (sig < 0 || sig >= NSIG)
  {
    errno = EINVAL;
    return -1;
  }

  if (old)
  {
    *old = __sig_actions[sig];
  }

  if (sa)
  {
    __sig_actions[sig] = *sa;
  }

  return 0;
}
