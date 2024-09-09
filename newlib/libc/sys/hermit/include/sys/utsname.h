// SPDX-FileCopyrightText: 2024 TriliTech <contact@trili.tech>
// SPDX-FileCopyrightText: 2005-2014 Rich Felker, et al.
// SPDX-License-Identifier: MIT

#ifndef	_SYS_UTSNAME_H
#define	_SYS_UTSNAME_H

#ifdef __cplusplus
extern "C" {
#endif

struct utsname {
	char sysname[65];
	char nodename[65];
	char release[65];
	char version[65];
	char machine[65];
#ifdef _GNU_SOURCE
	char domainname[65];
#else
	char __domainname[65];
#endif
};

int uname (struct utsname *);

#ifdef __cplusplus
}
#endif

#endif