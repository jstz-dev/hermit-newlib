// SPDX-FileCopyrightText: 2024 TriliTech <contact@trili.tech>
// SPDX-License-Identifier: MIT

#include <sched.h>
#include <reent.h>
#include <errno.h>
#include <hermit.h>

// TODO:
// Stub sched_{set/get}affinity
// Stub sched_cpucount

HERMIT_STUB(int, sched_get_priority_max, (int policy), -1)
HERMIT_STUB(int, sched_get_priority_min, (int policy), -1)

HERMIT_STUB(int, sched_getcpu, (void), -1)
HERMIT_STUB(int, sched_rr_get_interval, (pid_t pid, struct timespec *interval), -1)

HERMIT_STUB(int, sched_getparam, (pid_t pid, struct sched_param *param), -1)
HERMIT_STUB(int, sched_setparam, (pid_t pid, const struct sched_param *param), -1)

HERMIT_STUB(int, sched_getscheduler, (pid_t pid), -1)
HERMIT_STUB(int, sched_setscheduler, (pid_t pid, int policy, const struct sched_param *param), -1)

HERMIT_SYSCALL(int, sched_yield, (void), ())
