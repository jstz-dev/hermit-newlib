// SPDX-FileCopyrightText: 2024 TriliTech <contact@trili.tech>
// SPDX-License-Identifier: MIT

#include <reent.h>
#include <stdint.h>
#include <sys/param.h>
#include <stdalign.h>
#include <stddef.h>
#include <errno.h>
#include <hermit.h>

// Alignment is the number of bytes between successive addresses at which a given
// object can be allocated.
//
// Pointers returned by allocation functions, such as `malloc`, must be suitably
// aligned for any object, which means they are aligned at least as strictly as
// `max_align_t`.

// ALIGN_UP rounds `x` up to the nearest multiple of `align`, assuming `align` is
// a power of 2.
#define ALIGN_UP(x, align) (((x) + (align) - 1) & ~((align) - 1))

#define MAX_ALIGN_UP(x) ALIGN_UP(x, alignof(max_align_t))
#define MAX_ALIGN alignof(max_align_t)

#define IS_POWER_OF_TWO(x) (((x) & ((x) - 1)) == 0)

// The `block_t` structure is used to store the size of the allocated memory block.
// The size is aligned to `MAX_ALIGN` bytes to ensure that the pointer returned by
// the allocation functions is suitably aligned for any object.
typedef struct block 
{
  alignas(MAX_ALIGN) size_t size;
} block_t;

void *_malloc_r(struct _reent *reent, size_t size)
{
  size_t mem_size = MAX_ALIGN_UP(size);
  block_t *mem = sys_alloc(mem_size, MAX_ALIGN);

  if (mem == NULL)
  {
    return NULL;
  }

  mem->size = mem_size;
  return mem + 1;
}

void *_calloc_r(struct _reent *reent, size_t num, size_t size)
{
  size_t mem_size = MAX_ALIGN_UP(num * size);
  block_t *mem = sys_alloc_zeroed(mem_size, MAX_ALIGN);

  if (mem == NULL)
  {
    return NULL;
  }

  mem->size = mem_size;
  return mem + 1;
}

void *_realloc_r(struct _reent *reent, void *ptr, size_t new_size)
{
  size_t mem_size = MAX_ALIGN_UP(new_size);

  if (ptr == NULL)
  {
    return _malloc_r(reent, mem_size);
  }

  block_t *mem = (block_t *)ptr - 1;
  block_t *new_mem = sys_realloc(mem, mem->size, MAX_ALIGN, mem_size);

  if (new_mem == NULL)
  {
    return NULL;
  }

  new_mem->size = mem_size;
  return new_mem + 1;
}

// The `memalign` function is deprecated. Use `aligned_alloc` instead.
// This `memalign` implementation is solely for `posix_memalign` and `aligned_alloc`.
void *_memalign_r(struct _reent *_r, size_t align, size_t size) {
  if (!IS_POWER_OF_TWO(align)) {
    return NULL;
  }

  size_t mem_align = MAX(align, alignof(block_t));
  size_t mem_size = ALIGN_UP(size, mem_align); 
  block_t *mem = sys_alloc(mem_size, mem_align);

  if (mem == NULL)
  {
    return NULL;
  }

  mem->size = mem_size;
  return mem + 1;
}

void _free_r(struct _reent *reent, void *ptr)
{
  if (ptr == NULL)
  {
    return;
  }

  block_t *mem = (block_t *)ptr - 1;
  
  sys_dealloc(mem, mem->size, MAX_ALIGN);
}


void *malloc(size_t size)
{
  return _malloc_r(_REENT, size);
}

void *calloc(size_t num, size_t size)
{
  return _calloc_r(_REENT, num, size);
}

void *memalign(size_t align, size_t size)
{
  return _memalign_r(_REENT, align, size);
}

void *realloc(void *ptr, size_t new_size)
{
  return _realloc_r(_REENT, ptr, new_size);
}

void free(void *ptr)
{
  _free_r(_REENT, ptr);
}

int32_t posix_memalign(void **memptr, size_t align, size_t size)
{
  // The alignment must be a power of 2 and a multiple of `sizeof(void *)`
  if (align % sizeof(void *) != 0 || !IS_POWER_OF_TWO(align))
  {
    return EINVAL;
  }

  void *mem = memalign(align, size);
  if (mem == NULL)
  {
    return ENOMEM;
  }

  *memptr = mem;
  return 0;
}

// TODO: stub mallinfo, malloc_stats, mallopt, malloc_usable_size, valloc, pvalloc, malloc_trim, mstats