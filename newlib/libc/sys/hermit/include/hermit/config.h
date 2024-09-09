// SPDX-FileCopyrightText: 2024 TriliTech <contact@trili.tech>
// SPDX-License-Identifier: MIT

#ifndef _HERMIT_CONFIG_H
#define _HERMIT_CONFIG_H

#define HERMIT_SBRK_HEAP (8 * 1024 * 4096)

// Hermit lacks a proper notion of processes, users and groups.
//
// We assume (for simplicity) that there is:
// - one process (init, pid(1))
// - one user (root, uid(0))
// - one group (root, gid(0))
//
// All threads belong to init.
// The process group id of init is 0.

#define HERMIT_UID 0
#define HERMIT_GID 0
#define HERMIT_NGROUPS_MAX 1
#define HERMIT_NGROUPS 1
#define HERMIT_PGID 0
#define HERMIT_INIT_PID 1
#define HERMIT_INIT_PPID 0

#endif // _HERMIT_CONFIG_H