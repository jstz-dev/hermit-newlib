// SPDX-FileCopyrightText: 2024 TriliTech <contact@trili.tech>
// SPDX-License-Identifier: MIT

#include <reent.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/mman.h>

// Fixed length of 8 KB pages
// TODO: Make this configurable in <hermit/config.h>
#define SBRK_HEAP 8*1024*4096

uint8_t *heap;
uint8_t *brkp = NULL;
uint8_t *endp = NULL;

static void init()  {
	heap = (uint8_t *)mmap(0, SBRK_HEAP, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	brkp = heap;
	endp = heap + SBRK_HEAP;
}

void* _sbrk_r(struct _reent *reent, ptrdiff_t incr)
{
	if (brkp == NULL) {
		// Initialise the sbrk heap
		init();
	}

	if (incr == 0) {
		return (void*)brkp;
	}

	void *free = (void*)brkp;
	
	brkp += incr;
	if (brkp > endp) {
		reent->_errno = ENOMEM;
		return NULL;
	}

	return free;
}

void* sbrk (ptrdiff_t incr)
{
	return _sbrk_r(_REENT, incr);
}
