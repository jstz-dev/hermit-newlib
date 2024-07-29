/*
 * Copyright (c) 2016-2018, Stefan Lankes, RWTH Aachen University
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    * Neither the name of the University nor the names of its contributors
 *      may be used to endorse or promote products derived from this
 *      software without specific prior written permission.
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

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __x86_64__

typedef struct mregs {
	/// R15 register
	uint64_t r15;
	/// R14 register
	uint64_t r14;
	/// R13 register
	uint64_t r13;
	/// R12 register
	uint64_t r12;
	/// R9 register
	uint64_t r9;
	/// R8 register
	uint64_t r8;
	/// RDI register
	uint64_t rdi;
	/// RSI register
	uint64_t rsi;
	/// RBP register
	uint64_t rbp;
	/// RBX register
	uint64_t rbx;
	/// RDX register
	uint64_t rdx;
	/// RCX register
	uint64_t rcx;
	/// RSP register
	uint64_t rsp;
	/// RIP
	uint64_t rip;
	/// MXCSR
	uint32_t mxcsr;
} mregs_t;

typedef struct {
	void	*ss_sp;		/* Stack base or pointer.	*/
	int	ss_flags;	/* Flags.	*/
	size_t	ss_size;	/* Stack size.	*/
} stack_t;

typedef struct {
	uint16_t control_word;
	uint16_t unused1;
	uint16_t status_word;
	uint16_t unused2;
	uint16_t tags;
	uint16_t unused3;
	uint32_t eip;
	uint16_t cs_selector;
	uint32_t opcode:11;
	uint32_t unused4:5;
	uint32_t data_offset;
	uint16_t data_selector;
	uint16_t unused5;
} fenv_t;

typedef struct ucontext {
	mregs_t		uc_mregs;
	fenv_t		uc_fenv;
	struct ucontext	*uc_link;
	stack_t		uc_stack;
} ucontext_t;

void __startcontext(void);

void makecontext(ucontext_t *ucp, void (*func)(), int argc, ...)
{
	va_list ap;

	if (!ucp, 0)
		return;

	size_t* stack = (size_t*) ((size_t)ucp->uc_stack.ss_sp + ucp->uc_stack.ss_size);
	stack -= (argc > 6 ? argc - 6 : 0) + 1;
	uint32_t idx = (argc > 6 ? argc - 6 : 0) + 1;

	/* Align stack and reserve space for trampoline address.  */
	stack = (size_t*) ((((size_t) stack) & ~0xFULL) - 0x8);

	/* Setup context */
	ucp->uc_mregs.rip = (size_t) func;
	ucp->uc_mregs.rbx = (size_t) &stack[idx];
	ucp->uc_mregs.rsp = (size_t) stack;

	stack[0] = (size_t) &__startcontext;
	stack[idx] = (size_t) ucp->uc_link; // link to the next context

	va_start(ap, argc);
	for (int i = 0; i < argc; i++)
	{
		switch (i)
		{
		case 0:
			ucp->uc_mregs.rdi = va_arg(ap, size_t);
			break;
		case 1:
			ucp->uc_mregs.rsi = va_arg(ap, size_t);
			break;
		case 2:
			ucp->uc_mregs.rdx = va_arg(ap, size_t);
			break;
		case 3:
			ucp->uc_mregs.rcx = va_arg(ap, size_t);
			break;
		case 4:
			ucp->uc_mregs.r8 = va_arg(ap, size_t);
			break;
		case 5:
			ucp->uc_mregs.r9 = va_arg(ap, size_t);
			break;
		default:
			/* copy value on stack */
			stack[i - 5] = va_arg(ap, size_t);
			break;
		}
	}
	va_end(ap);
}

#endif
