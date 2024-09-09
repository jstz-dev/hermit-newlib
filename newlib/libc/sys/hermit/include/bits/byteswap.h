// SPDX-FileCopyrightText: 2024 TriliTech <contact@trili.tech>
// SPDX-License-Identifier: MIT

// This file is to be kept in sync with winsup/cygwin/include/bits/byteswap.h

#ifndef _BITS_BYTESWAP_H
#define _BITS_BYTESWAP_H

#ifdef __cplusplus
extern "C" {
#endif

static __inline unsigned short
__bswap_16 (unsigned short __x)
{
  return (__x >> 8) | (__x << 8);
}

static __inline unsigned int
__bswap_32 (unsigned int __x)
{
  return (__bswap_16 (__x & 0xffff) << 16) | (__bswap_16 (__x >> 16));
}

static __inline unsigned long long
__bswap_64 (unsigned long long __x)
{
  return (((unsigned long long) __bswap_32 (__x & 0xffffffffull)) << 32) | (__bswap_32 (__x >> 32));
}

#ifdef __cplusplus
}
#endif
#endif /* _BITS_BYTESWAP_H */
