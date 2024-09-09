// SPDX-FileCopyrightText: 2024 TriliTech <contact@trili.tech>
// SPDX-License-Identifier: MIT

#ifndef _SYS_DIRENT_H
#define _SYS_DIRENT_H

#include <sys/types.h>

#define DT_UNKNOWN 0
#define DT_FIFO 1
#define DT_CHR 2
#define DT_DIR 4
#define DT_BLK 6
#define DT_REG 8
#define DT_LNK 10
#define DT_SOCK 12
#define DT_WHT 14

typedef struct _dirdesc
{
	int dd_fd;
	long dd_loc;
	long dd_size;
	char *dd_buf;
	int dd_len;
	long dd_seek;
} DIR;

#define __dirfd(dp) ((dp)->dd_fd)

DIR *opendir(const char *);
struct dirent *readdir(DIR *);
void rewinddir(DIR *);
int closedir(DIR *);

typedef struct dirent
{
	uint64_t d_ino;
	int64_t d_off;
	uint16_t d_reclen;
	uint8_t d_type;
	char d_name[256];
} dirent_t;

#endif
