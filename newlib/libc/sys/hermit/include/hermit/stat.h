// SPDX-FileCopyrightText: 2024 TriliTech <contact@trili.tech>
// SPDX-License-Identifier: MIT

#ifndef _HERMIT_STAT_H
#define _HERMIT_STAT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/types.h>
#include <sys/timespec.h>

struct stat
{
  uint64_t st_dev;
  uint64_t st_ino;
  uint64_t st_nlink;
  /**
   * access permissions
   */
  uint32_t st_mode;
  /**
   * user id
   */
  uint32_t st_uid;
  /**
   * group id
   */
  uint32_t st_gid;
  /**
   * device id
   */
  uint64_t st_rdev;
  /**
   * size in bytes
   */
  uint64_t st_size;
  /**
   * block size
   */
  int64_t st_blksize;
  /**
   * size in blocks
   */
  int64_t st_blocks;
  /**
   * time of last access
   */
  struct timespec st_atim;
  /**
   * time of last modification
   */
  struct timespec st_mtim;
  /**
   * time of last status change
   */
  struct timespec st_ctim;
};


#ifdef __cplusplus
}
#endif

#endif // _HERMIT_STAT_H