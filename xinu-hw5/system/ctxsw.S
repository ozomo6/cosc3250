/**
 * @file     ctxsw.s
 * @provides ctxsw
 *
 * COSC 3250 / COEN 4820 Assignment 4
 */
/* Embedded XINU, Copyright (C) 2025.  All rights reserved. */

#include <riscv.h>

.text
	.align 4
	.globl	ctxsw


/**
 * @fn void ctxsw(&oldcontext, &newcontext)
 *
 * Switch context (values in registers) to another process, saving the
 * current processes information. This function will not return as normally
 * thought as it will load in the program counter for a different process and
 * jump to that location and begin executing code.
 *
 * @param  &oldcontext address of outgoing process context save area
 * @param  &newcontext address of incoming process context save area
 * @return special case -- see above
 */
	.func ctxsw
ctxsw:

// TODO: Save process state, including at least the 
//	callee-save ("non-volatile") registers.

	//push s registers onto a0 register
	
	/*sw s0,  0(a0)  #s0 stored on a0 reg
	sw s1,  4(a0)  #s1 stored on a0 reg
	sw s2,  8(a0)  #s2 stored on a0 reg
        sw s3,  12(a0) #s3 stored on a0 reg
	sw s4,  16(a0) #s4 stored on a0 reg
        sw s5,  20(a0) #s5 stored on a0 reg
        sw s6,  24(a0) #s6 stored on a0 reg
        sw s7,  28(a0) #s7 stored on a0 reg
	sw s8,  32(a0) #s8 stored on a0 reg
        sw s9,  36(a0) #s9 stored on a0 reg
        sw s10, 40(a0) #s10 stored on a0 reg
        sw s11, 44(a0) #s11 stored on a0 reg

	//store return address

	sw ra, 48(a0)  #ret addr on a0 reg

	sw sp, 52(

	*/

// TODO: Switch from old process stack to new process stack.
/*
	lw s0, 0(a1)   #s0 loaded from a1 reg
	lw s1, 4(a1)   #s1 loaded from a1 reg
	lw s2, 8(a1)   #s2 loaded from a1 reg
	lw s3, 12(a1)  #s3 loaded from a1 reg
	lw s4, 16(a1)  #s4 loaded from a1 reg
        lw s5, 20(a1)  #s6 loaded from a1 reg
        lw s6, 24(a1)  #s6 loaded from a1 reg
        lw s7, 28(a1)  #s7 loaded from a1 reg
        lw s8, 32(a1)  #s8 loaded from a1 reg
        lw s9, 36(a1)  #s9 loaded from a1 reg
        lw s10, 40(a1) #s10 loaded from a1 reg
        lw s11, 44(a1) #s11 loaded from a1 reg
*/

        # Save the old process context (callee-saved registers)
        sd s0,  CTX_S0*8(a0)  # Save s0
        sd s1,  CTX_S1*8(a0)  # Save s1
        sd s2,  CTX_S2*8(a0)  # Save s2
        sd s3,  CTX_S3*8(a0)  # Save s3
        sd s4,  CTX_S4*8(a0)  # Save s4
        sd s5,  CTX_S5*8(a0)  # Save s5
        sd s6,  CTX_S6*8(a0)  # Save s6
        sd s7,  CTX_S7*8(a0)  # Save s7
        sd s8,  CTX_S8*8(a0)  # Save s8
        sd s9,  CTX_S9*8(a0)  # Save s9
        sd s10, CTX_S10*8(a0) # Save s10
        sd s11, CTX_S11*8(a0) # Save s11
        sd sp,  CTX_SP*8(a0)  # Save stack pointer (SP)
        sd ra,  CTX_RA*8(a0)  # Save return address (RA)

        # Load the new process's stack pointer
        ld sp,  CTX_SP*8(a1)  # Restore stack pointer from new context
        ld t0,  CTX_SP*8(a1)  # Load new stack pointer (optional double-check)

        # Restore the new process's context (callee-saved registers)
        ld s0,  CTX_S0*8(a1)  # Restore s0
        ld s1,  CTX_S1*8(a1)  # Restore s1
        ld s2,  CTX_S2*8(a1)  # Restore s2
        ld s3,  CTX_S3*8(a1)  # Restore s3
        ld s4,  CTX_S4*8(a1)  # Restore s4
        ld s5,  CTX_S5*8(a1)  # Restore s5
        ld s6,  CTX_S6*8(a1)  # Restore s6
        ld s7,  CTX_S7*8(a1)  # Restore s7
        ld s8,  CTX_S8*8(a1)  # Restore s8
        ld s9,  CTX_S9*8(a1)  # Restore s9
        ld s10, CTX_S10*8(a1) # Restore s10
        ld s11, CTX_S11*8(a1) # Restore s11
        ld ra,  CTX_RA*8(a1)  # Restore return address (RA)

        # Load the program counter of the new process
        ld t0,  CTX_PC*8(a1)  # Load new process's program counter

        # Jump to the restored process execution
        jalr zero, t0         # Jump to the new process

        .end ctxsw



// TODO: Restore process state, including at least the
//	callee-save ("non-volatile") registers.
//	These call and restore segments must match the register data
//	layout you choose in create().
// TODO: Jump to next function.
//	Note: there are two important values at play here --
//	the address of the process's function that we want to end up in
//	once we leave this ctxsw(), and the value we want to have in the
//	return address register.  Those are almost always the same value, except
//	for the very first time we ctxsw() to a new process, when they
//	are different.  See create() for more details.

// Note: This ret line immediately returns from whence we came.  This
//	is here as a placeholder so that you don't crash your O/S when
//	you call ctxsw() before it is complete.  But, this line of code
//	is not actually a part of a completed solution.
	ret

// ASSIGNMENT 5 TODO:
// Note: the following code is for the next assignment.  DO NOT uncomment it
//      until you believe your trap handler is working.
// Determine which privilege mode the process should be in
//	ld t0, (**REGISTER THAT HOLDS NEW CONTEXT PROGRAM COUNTER**)
//	beq t0, ra, switch
//	li t4, SSTATUS_S_MODE
//	csrc sstatus, t4
//	csrw sepc, t0
//
//	sret
//switch:
//	ret

	.end ctxsw
