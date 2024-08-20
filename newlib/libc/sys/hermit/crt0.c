/*
 * Copyright (c) 2016, Stefan Lankes, RWTH Aachen University
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of the University nor the names of its contributors
 *     may be used to endorse or promote products derived from this
 *     software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <reent.h>
#include <dirent.h>
#include "syscall.h"


extern int main(int argc, char** argv);
extern void __libc_init_array(void);
extern void __libc_fini_array (void);
extern int _init_signal(void);
extern char** environ;
extern void   __sinit (struct _reent *);
extern struct _glue __sglue;
extern __FILE __sf[3];

typedef struct layout {
   size_t size;
} layout_t;

#define HERMIT_MALLOC_ALIGN ((size_t)8)

void* malloc(size_t s) {
   return _malloc_r(_REENT, s);
}

void* _malloc_r(struct _reent* _r, size_t size) {
   uint8_t *ptr = sys_malloc(size + sizeof(layout_t), HERMIT_MALLOC_ALIGN);
   if (!ptr)
      return NULL;

   ((layout_t*)ptr)->size = size;
   return (void*)(ptr + sizeof(layout_t));
}

void* calloc(size_t nmemb, size_t size) {
   return _calloc_r(_REENT, nmemb, size);
}

void* _calloc_r(struct _reent* r, size_t nmemb, size_t size) {
   return NULL;
}

void* realloc(void* ptr, size_t size) {
   return _realloc_r(_REENT, ptr, size);
}

void* _realloc_r(struct _reent* r, void* ptr, size_t size) {
   return NULL;
}

void free(void* ptr) {
   _free_r(_REENT, ptr);
}

void _free_r(struct _reent* r, void* ptr) {
   if (ptr) {
      uint8_t* p = (uint8_t*)ptr - sizeof(layout_t);
      sys_free(p, ((layout_t*)p)->size + sizeof(layout_t), HERMIT_MALLOC_ALIGN);
   }
}

int getentropy(void *ptr, __size_t n) { return -1; };

int32_t mkdir(const char *name, uint32_t mode) {
   return sys_mkdir(name, mode);
}

int chdir(const char *path) {
   return -1;
}

int fchdir(int fd) {
   return -1;
}

int chmod(const char *path, mode_t mode) {
   return -1;
}

int fchmod(int fd, mode_t mode) {
   return -1;
}

int chown(const char *path, uid_t owner, gid_t group) {
   return -1;
}

int fchown(int fd, uid_t owner, gid_t group) {
   return -1;
}

int truncate(const char *path, off_t length) {
   return -1;
}
int ftruncate(int fd, off_t length) { 
   return -1; 
}

long fpathconf (int fd, int name) {
   return -1;
}

long pathconf (const char *path, int name) {
   return -1;
}

char *getcwd (char *__buf, size_t __size) {
   return NULL;
}

int getdents(int fd, void *dp, int count) { return -1; }

int _rename_r (struct _reent *r, const char *a, const char *b) { return -1; }



void runtime_entry(int argc, char** argv, char** env)
{
   int ret;

   /* call init function */
   __libc_init_array();

   /* register a function to be called at normal process termination */
   atexit(__libc_fini_array);

   /* optind is the index of the next element to be processed in argv */
   optind = 0;

   if (env)
      environ = env;

   /* initialize simple signal handling */
   //_init_signal();

   ret = main(argc, argv);

   /* call exit from the C library so atexit gets called, and the
      C++ destructors get run. This calls our exit routine below    
      when it's done. */
   exit(ret);

   /* we should never reach this point */
   while(1) {}
}
