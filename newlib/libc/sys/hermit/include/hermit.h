// SPDX-FileCopyrightText: 2024 TriliTech <contact@trili.tech>
// SPDX-License-Identifier: MIT

#ifndef _HERMIT_H
#define _HERMIT_H

#include <hermit/syscall.h>
#include <hermit/config.h>

#define ARG_TYPES(...) __VA_ARGS__
#define ARG_NAMES(...) __VA_ARGS__

#define HERMIT_STUB_WITH_ERRNO(return_type, name, arg_types, errno_value, fail_value) \
  return_type name(ARG_TYPES arg_types)                                               \
  {                                                                                   \
    errno = errno_value;                                                              \
    return fail_value;                                                                \
  }

#define HERMIT_STUB(return_type, name, arg_types, fail_value) \ 
    HERMIT_STUB_WITH_ERRNO(return_type, name, arg_types, ENOSYS, fail_value)

#define HERMIT_STUB_REENT_WITH_ERRNO(return_type, name, arg_types, arg_names, errno_value, fail_value) \
  return_type _##name##_r(struct _reent *r, ARG_TYPES arg_types)                                       \
  {                                                                                                    \
    r->_errno = errno_value;                                                                           \
    return fail_value;                                                                                 \
  }                                                                                                    \
  return_type name(ARG_TYPES arg_types)                                                                \
  {                                                                                                    \
    return _##name##_r(_REENT, ARG_NAMES arg_names);                                                   \
  }

#define HERMIT_STUB_REENT(return_type, name, arg_types, arg_names, fail_value) \ 
    HERMIT_STUB_REENT_WITH_ERRNO(return_type, name, arg_types, arg_names, ENOSYS, fail_value)

#define HERMIT_STUB_REENT0_WITH_ERRNO(return_type, name, errno_value, fail_value) \
  return_type _##name##_r(struct _reent *r)                                       \
  {                                                                               \
    r->_errno = errno_value;                                                      \
    return fail_value;                                                            \
  }                                                                               \
  return_type name()                                                              \
  {                                                                               \
    return _##name##_r(_REENT);                                                   \
  }

#define HERMIT_STUB_REENT0(return_type, name, fail_value) \
  HERMIT_STUB_REENT0_WITH_ERRNO(return_type, name, ENOSYS, fail_value)

#define HERMIT_SYSCALL(return_type, name, arg_types, arg_names) \
  return_type name(ARG_TYPES arg_types)                         \
  {                                                             \
    return_type ret = sys_##name(ARG_NAMES arg_names);          \
    if (ret < 0)                                                \
    {                                                           \
      errno = -ret;                                             \
      return -1;                                                \
    }                                                           \
    return ret;                                                 \
  }

#define HERMIT_SYSCALL_REENT(return_type, name, arg_types, arg_names) \
  return_type _##name##_r(struct _reent *reent, ARG_TYPES arg_types)  \
  {                                                                   \
    return_type ret = sys_##name(ARG_NAMES arg_names);                \
    if (ret < 0)                                                      \
    {                                                                 \
      reent->_errno = -ret;                                           \
      return -1;                                                      \
    }                                                                 \
    return ret;                                                       \
  }                                                                   \
  return_type name(ARG_TYPES arg_types)                               \
  {                                                                   \
    return _##name##_r(_REENT, ARG_NAMES arg_names);                  \
  }

#endif // _HERMIT_H