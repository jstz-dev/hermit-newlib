/*
 * signal.c
 * Original Author:	G. Haley
 *
 * signal associates the function pointed to by func with the signal sig. When
 * a signal occurs, the value of func determines the action taken as follows:
 * if func is SIG_DFL, the default handling for that signal will occur; if func
 * is SIG_IGN, the signal will be ignored; otherwise, the default handling for
 * the signal is restored (SIG_DFL), and the function func is called with sig
 * as its argument. Returns the value of func for the previous call to signal
 * for the signal sig, or SIG_ERR if the request fails.
 */

/* _init_signal initialises the signal handlers for each signal. This function
   is called by crt0 at program startup.  */


/*
 * Adapted for HermitCore by:
 *  - S. Lankes, RWTH Aachen University
 *  - Daniel Krebs, RWTH Aachen University
 */

#include <errno.h>
#include <signal.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <reent.h>
#include <malloc.h>
#include <_syslist.h>
#include "syscall.h"

/*
 * Notes:
 *	- man 2 sigreturn
 *	- man 7 signal
*/

#if 0
static void
newlib_signal_dispatcher(int signum)
{
	// assume to be called in user context
	struct _reent* reent = _REENT;

	if (signum < 0 || signum >= NSIG) {
		reent->_errno = EINVAL;
		return;
	}

	if(reent->_sig_func == NULL) {
		reent->_errno = ENOENT;
		return;
	}

	_sig_func_ptr func = reent->_sig_func[signum];

	if(func == SIG_DFL) {
		fprintf(stderr, "Caught unhandled signal %d, terminating\n", signum);
		// terminate if no signal handler registered
		sys_exit(128 + signum);
	} else if(func == SIG_IGN) {
		// ignore
	} else if(func == SIG_ERR) {
		reent->_errno = EINVAL;
	} else {
		// finally call user code
		func(signum);
	}
}
#endif

int
pthread_sigmask (int how, const sigset_t* set, sigset_t* oset)
{
	return ENOSYS;
}

int
sigaltstack (const stack_t* ss, stack_t* oss)
{
	return ENOSYS;
}

int
_init_signal_r (struct _reent* ptr)
{
#if 0
	int i;

	if (ptr->_sig_func == NULL) {
		ptr->_sig_func = (_sig_func_ptr *)_malloc_r (ptr, sizeof (_sig_func_ptr) * NSIG);

		if (ptr->_sig_func == NULL) {
			return -1;
		}

		for (i = 0; i < NSIG; i++) {
			ptr->_sig_func[i] = SIG_DFL;
		}

		sys_signal(newlib_signal_dispatcher);
	}

	return 0;
#else
	return ENOSYS;
#endif
}

_sig_func_ptr
_signal_r (struct _reent* ptr, int sig, _sig_func_ptr func)
{
#if 0
	_sig_func_ptr old_func;

	if (sig < 0 || sig >= NSIG) {
		ptr->_errno = EINVAL;
		return SIG_ERR;
	}

	if (ptr->_sig_func == NULL && _init_signal_r (ptr) != 0) {
		return SIG_ERR;
	}

	old_func = ptr->_sig_func[sig];
	ptr->_sig_func[sig] = func;

	return old_func;
#else
	return ENOSYS;
#endif
}

int
_sigaction_r (struct _reent* ptr, int sig, const struct sigaction* act, struct sigaction* oact)
{
	return 0;
}

#ifndef _REENT_ONLY

int
sigaction (int sig, const struct sigaction* act, struct sigaction* oact)
{
	return _sigaction_r (_REENT, sig, act, oact);
}

_sig_func_ptr
signal (int sig, _sig_func_ptr func)
{
	return _signal_r (_REENT, sig, func);
}

#if 0
int 
_init_signal (void)
{
	return _init_signal_r (_REENT);
}
#endif

#endif
