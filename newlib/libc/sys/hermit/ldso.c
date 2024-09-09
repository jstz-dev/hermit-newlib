// SPDX-FileCopyrightText: 2024 TriliTech <contact@trili.tech>
// SPDX-License-Identifier: MIT

#include <reent.h>
#include <errno.h>
#include <hermit.h>

HERMIT_STUB(int, dlclose, (void *handle), -1)
HERMIT_STUB(char*, dlerror, (void), NULL)
HERMIT_STUB(void*, dlopen, (const char *filename, int flags), NULL)
HERMIT_STUB(void*, dlsym, (void *handle, const char *symbol), NULL)
