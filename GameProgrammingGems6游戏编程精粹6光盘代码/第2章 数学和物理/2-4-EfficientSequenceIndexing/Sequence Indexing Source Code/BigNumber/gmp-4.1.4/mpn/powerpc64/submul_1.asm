dnl  PowerPC-64 mpn_submul_1 -- Multiply a limb vector with a limb and subtract
dnl  the result from a second limb vector.

dnl  Copyright 1999, 2000, 2001 Free Software Foundation, Inc.

dnl  This file is part of the GNU MP Library.

dnl  The GNU MP Library is free software; you can redistribute it and/or modify
dnl  it under the terms of the GNU Lesser General Public License as published
dnl  by the Free Software Foundation; either version 2.1 of the License, or (at
dnl  your option) any later version.

dnl  The GNU MP Library is distributed in the hope that it will be useful, but
dnl  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
dnl  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
dnl  License for more details.

dnl  You should have received a copy of the GNU Lesser General Public License
dnl  along with the GNU MP Library; see the file COPYING.LIB.  If not, write to
dnl  the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
dnl  MA 02111-1307, USA.

include(`../config.m4')

C INPUT PARAMETERS
C res_ptr	r3
C s1_ptr	r4
C size		r5
C s2_limb	r6
C cy_limb	r7

C PPC630: 6 to 18 cycles/limb, depending on multiplier.  This cannot be
C improved unless floating-point operations are used instead of the slow
C mulld/mulhdu.

ASM_START()
PROLOGUE(mpn_submul_1)
	li	r7,0			C cy_limb = 0

PROLOGUE(mpn_submul_1c)
	mtctr	r5
	addic	r0,r0,0
	addi	r3, r3, -8
	addi	r4, r4, -8
.Loop:
	ldu	r0,8(r4)
	ld	r10,8(r3)
	mulld	r9,r0,r6
	adde	r9,r9,r7
	mulhdu	r7,r0,r6
	addze	r7,r7
	subfc	r9,r9,r10
	stdu	r9,8(r3)
	subfe	r11,r11,r11		C invert ...
	addic	r11,r11,1		C ... carry
	bdnz	.Loop

	addze	r3,r7
	blr
EPILOGUE(mpn_submul_1)
EPILOGUE(mpn_submul_1c)
