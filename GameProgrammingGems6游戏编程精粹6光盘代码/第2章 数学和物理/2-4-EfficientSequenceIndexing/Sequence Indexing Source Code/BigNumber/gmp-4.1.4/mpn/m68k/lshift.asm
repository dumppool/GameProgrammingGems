dnl  mc68020 mpn_lshift -- mpn left shift.
dnl
dnl            cycles/limb
dnl         shift==1  shift>1
dnl  68040:   3.5       9.5

dnl  Copyright 1996, 1999, 2000, 2001, 2002, 2003 Free Software Foundation,
dnl  Inc.
dnl
dnl  This file is part of the GNU MP Library.
dnl
dnl  The GNU MP Library is free software; you can redistribute it and/or
dnl  modify it under the terms of the GNU Lesser General Public License as
dnl  published by the Free Software Foundation; either version 2.1 of the
dnl  License, or (at your option) any later version.
dnl
dnl  The GNU MP Library is distributed in the hope that it will be useful,
dnl  but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
dnl  Lesser General Public License for more details.
dnl
dnl  You should have received a copy of the GNU Lesser General Public
dnl  License along with the GNU MP Library; see the file COPYING.LIB.  If
dnl  not, write to the Free Software Foundation, Inc., 59 Temple Place -
dnl  Suite 330, Boston, MA 02111-1307, USA.

include(`../config.m4')


C INPUT PARAMETERS
C res_ptr	(sp + 4)
C s_ptr		(sp + 8)
C s_size	(sp + 12)
C cnt		(sp + 16)

define(res_ptr, `a1')
define(s_ptr,   `a0')
define(s_size,  `d6')
define(cnt,     `d4')

PROLOGUE(mpn_lshift)
C Save used registers on the stack.
	moveml	d2-d6/a2, M(-,sp)

C Copy the arguments to registers.
	movel	M(sp,28), res_ptr
	movel	M(sp,32), s_ptr
	movel	M(sp,36), s_size
	movel	M(sp,40), cnt

	moveql	#1, d5
	cmpl	d5, cnt
	bne	L(Lnormal)
	cmpl	s_ptr, res_ptr
	bls	L(Lspecial)		C jump if s_ptr >= res_ptr

ifelse(scale_available_p,1,`
	lea	M(s_ptr,s_size,l,4), a2
',`
	movel	s_size, d0
	asll	#2, d0
	lea	M(s_ptr,d0,l), a2
')
	cmpl	res_ptr, a2
	bls	L(Lspecial)		C jump if res_ptr >= s_ptr + s_size

L(Lnormal):
	moveql	#32, d5
	subl	cnt, d5

ifelse(scale_available_p,1,`
	lea	M(s_ptr,s_size,l,4), s_ptr
	lea	M(res_ptr,s_size,l,4), res_ptr
',`
	movel	s_size, d0
	asll	#2, d0
	addl	d0, s_ptr
	addl	d0, res_ptr
')
	movel	M(-,s_ptr), d2
	movel	d2, d0
	lsrl	d5, d0		C compute carry limb

	lsll	cnt, d2
	movel	d2, d1
	subql	#1, s_size
	beq	L(Lend)
	lsrl	#1, s_size
	bcs	L(L1)
	subql	#1, s_size

L(Loop:)
	movel	M(-,s_ptr), d2
	movel	d2, d3
	lsrl	d5, d3
	orl	d3, d1
	movel	d1, M(-,res_ptr)
	lsll	cnt, d2
L(L1:)
	movel	M(-,s_ptr), d1
	movel	d1, d3
	lsrl	d5, d3
	orl	d3, d2
	movel	d2, M(-,res_ptr)
	lsll	cnt, d1

	dbf	s_size, L(Loop)
	subl	#0x10000, s_size
	bcc	L(Loop)

L(Lend:)
	movel	d1, M(-,res_ptr)	C store least significant limb

C Restore used registers from stack frame.
	moveml	M(sp,+), d2-d6/a2
	rts

C We loop from least significant end of the arrays, which is only
C permissable if the source and destination don't overlap, since the
C function is documented to work for overlapping source and destination.

L(Lspecial):
	clrl	d0			C initialize carry
	eorw	#1, s_size
	lsrl	#1, s_size
	bcc	L(LL1)
	subql	#1, s_size

L(LLoop):
	movel	M(s_ptr,+), d2
	addxl	d2, d2
	movel	d2, M(res_ptr,+)
L(LL1):
	movel	M(s_ptr,+), d2
	addxl	d2, d2
	movel	d2, M(res_ptr,+)

	dbf	s_size, L(LLoop)
	addxl	d0, d0		C save cy in lsb
	subl	#0x10000, s_size
	bcs	L(LLend)
	lsrl	#1, d0		C restore cy
	bra	L(LLoop)

L(LLend):
C Restore used registers from stack frame.
	moveml	M(sp,+), d2-d6/a2
	rts

EPILOGUE(mpn_lshift)
