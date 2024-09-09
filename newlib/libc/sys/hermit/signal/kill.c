// SPDX-FileCopyrightText: 2024 TriliTech <contact@trili.tech>
// SPDX-License-Identifier: MIT

#include <reent.h>
#include <errno.h>
#include <sys/signal.h>
#include <hermit.h>

int kill(int pid, int sig)
{
	return _kill_r(_REENT, pid, sig);
}

int _kill_r(struct _reent *reent, int pid, int sig)
{

	if (pid == _getpid_r(reent))
	{
		return _raise_r(reent, sig);
	}

	reent->_errno = EPERM;
	return -1;
}
