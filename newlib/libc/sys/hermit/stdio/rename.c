// SPDX-FileCopyrightText: 2024 TriliTech <contact@trili.tech>
// SPDX-License-Identifier: MIT

#include <reent.h>
#include <errno.h>
#include <hermit.h>

HERMIT_STUB_REENT(int, rename, (const char *old, const char *new), (old, new), -1)