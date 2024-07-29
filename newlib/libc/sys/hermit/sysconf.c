// Copyright (c) 2018 Colin Finck, RWTH Aachen University
//
// MIT License
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include <unistd.h>
#include <limits.h>
#include <time.h>
#include <errno.h>
#include <pthread.h>
#include "syscall.h"

long
sysconf (int name)
{
	switch (name)
	{
		case _SC_ARG_MAX: return ARG_MAX;
		case _SC_CLK_TCK: return CLK_TCK;
		case _SC_NPROCESSORS_CONF: return sys_get_processor_count();
		case _SC_NPROCESSORS_ONLN: return sys_get_processor_count();
		case _SC_PAGESIZE: return sys_getpagesize();
		case _SC_PRIORITY_SCHEDULING: return 1;
		case _SC_THREAD_DESTRUCTOR_ITERATIONS: return PTHREAD_DESTRUCTOR_ITERATIONS;
		case _SC_THREAD_KEYS_MAX: return PTHREAD_KEYS_MAX;
		case _SC_THREAD_STACK_MIN: return PTHREAD_STACK_MIN;
		case _SC_THREAD_THREADS_MAX: return PTHREAD_THREADS_MAX;
		case _SC_THREADS: return 1;
		case _SC_TIMERS: return 1;
		case _SC_THREAD_ATTR_STACKADDR: return -1;
		case _SC_THREAD_ATTR_STACKSIZE: return 1;
		case _SC_THREAD_PRIORITY_SCHEDULING: return -1;
		case _SC_THREAD_PRIO_INHERIT: return -1;
		case _SC_THREAD_PRIO_PROTECT: return -1;
		case _SC_THREAD_PROCESS_SHARED: return -1;
		case _SC_THREAD_SAFE_FUNCTIONS: return 1;
		case _SC_VERSION: return _POSIX_VERSION;
		default:
			*__errno() = EINVAL;
			return -1;
	}
}
