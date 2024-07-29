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

#ifdef __x86_64__

// Required for Go applications
.globl getcontext
getcontext:
    cli
    // save general purpose regsiters
    mov %r15, 0x00(%rdi)
    mov %r14, 0x08(%rdi)
    mov %r13, 0x10(%rdi)
    mov %r12, 0x18(%rdi)
    mov %r9, 0x20(%rdi)
    mov %r8, 0x28(%rdi)
    mov %rdi, 0x30(%rdi)
    mov %rsi, 0x38(%rdi)
    mov %rbp, 0x40(%rdi)
    mov %rbx, 0x48(%rdi)
    mov %rdx, 0x50(%rdi)
    mov %rcx, 0x58(%rdi)
    lea 0x08(%rsp), %rax
    mov %rax, 0x60(%rdi)
    mov (%rsp), %rax
    mov %rax, 0x68(%rdi)
    // save FPU state
    fnstenv 0x74(%rdi)
    lea 0x70(%rdi), %rax
    stmxcsr (%rax)
    xor %rax, %rax
    sti
    retq

// Required for Go applications
.globl setcontext
setcontext:
    cli
    // restore FPU state
    fldenv 0x74(%rdi)
    lea 0x70(%rdi), %rax
    ldmxcsr (%rax)
    // restore general purpose registers
    mov 0x00(%rdi), %r15
    mov 0x08(%rdi), %r14
    mov 0x10(%rdi), %r13
    mov 0x18(%rdi), %r12
    mov 0x20(%rdi), %r9
    mov 0x28(%rdi), %r8
    mov 0x30(%rdi), %rdi
    mov 0x38(%rdi), %rsi
    mov 0x40(%rdi), %rbp
    mov 0x48(%rdi), %rbx
    mov 0x50(%rdi), %rdx
    mov 0x58(%rdi), %rcx
    mov 0x60(%rdi), %rsp
    pushq 0x68(%rdi)
    xor %rax, %rax
    sti
    retq

// Required for Go applications
.globl __startcontext
__startcontext:
    mov %rbx, %rsp
    pop %rdi
    cmp $0, %rdi
    je Lno_context

    callq setcontext

Lno_context:
    callq exit
Lendless:
    jmp Lendless

#endif

#if defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
