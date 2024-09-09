// SPDX-FileCopyrightText: 2024 TriliTech <contact@trili.tech>
// SPDX-FileCopyrightText: 2005-2014 Rich Felker, et al.
// SPDX-License-Identifier: MIT

#ifndef _SYS_UIO_H
#define _SYS_UIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/types.h>

#define UIO_MAXIOV 1024

struct iovec {
	void *iov_base;
	size_t iov_len;
};

ssize_t readv(int fd, const struct iovec *iov, int iovcnt);
ssize_t writev(int fd, const struct iovec *iov, int iovcnt);

#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
ssize_t preadv(int fd, const struct iovec *iov, int iovcnt, off_t offset);
ssize_t pwritev(int fd, const struct iovec *iov, int iovcnt, off_t offset);
#if defined(_LARGEFILE64_SOURCE) || defined(_GNU_SOURCE)
#define preadv64 preadv
#define pwritev64 pwritev
#define off64_t off_t
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif
