// SPDX-FileCopyrightText: 2024 TriliTech <contact@trili.tech>
// SPDX-License-Identifier: MIT

#include <reent.h>
#include <errno.h>
#include <sys/select.h>
#include <hermit.h>

HERMIT_STUB(int, select, (int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout), -1)
HERMIT_SYSCALL(int, poll, (struct pollfd * fds, nfds_t nfds, int timeout), (fds, nfds, timeout))