// SPDX-FileCopyrightText: 2024 TriliTech <contact@trili.tech>
// SPDX-License-Identifier: MIT

#include <reent.h>
#include <errno.h>
#include <sys/types.h>
#include <hermit.h>

HERMIT_STUB_REENT0(int, fork, -1)
HERMIT_STUB(int, vfork, (void), -1)
HERMIT_STUB(int, fexecve, (int fd, char *const argv[], char *const envp[]), -1)
HERMIT_STUB(int, _execve, (const char *path, char *const argv[], char *const envp[]), -1)
HERMIT_STUB_REENT(int, wait, (int *status), (status), -1)
HERMIT_STUB(int, waitpid, (pid_t pid, int *status, int options), -1)
