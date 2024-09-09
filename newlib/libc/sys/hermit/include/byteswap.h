// SPDX-FileCopyrightText: 2024 TriliTech <contact@trili.tech>
// SPDX-License-Identifier: MIT

// This file is to be kept in sync with winsup/cygwin/include/byteswap.h

#ifndef _BYTESWAP_H
#define _BYTESWAP_H

#include <bits/byteswap.h>

#define bswap_16(x) __bswap_16(x)
#define bswap_32(x) __bswap_32(x)
#define bswap_64(x) __bswap_64(x)

#endif /* _BYTESWAP_H */