dnl  IA-64 mpn_sub_n -- Subtract two limb vectors of the same length > 0 and
dnl  store difference in a third limb vector.

dnl  Copyright 2000, 2001, 2002 Free Software Foundation, Inc.

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
C rp = r32
C up = r33
C vp = r34
C n = r35

C         cycles/limb
C Itanium:    6
C Itanium 2:  3


ASM_START()
PROLOGUE(mpn_sub_n)
	.prologue
ifdef(`HAVE_ABI_32',
`		addp4	r32 = 0, r32
		addp4	r33 = 0, r33
		addp4	r34 = 0, r34
		sxt4	r35 = r35
		;;
')
  { .mib;	cmp.eq		p6, p7 = 1, r35
		add		r35 = -2, r35
		nop.b		0
} { .mib;	cmp.ne		p8, p9 = r0, r0
	.save	ar.lc, r2
		mov		r2 = ar.lc
	.body
		nop.b		0			;;
} { .mib;	ld8		r16 = [r33], 8
		mov		ar.lc = r35
		nop.b		0
} { .mib;	ld8		r17 = [r34], 8
		nop.i		0
	   (p6)	br.dptk		.Lend			;;
}
		.align	32
.Loop:
	.pred.rel "mutex",p8,p9
  { .mii;	mov		r20 = r16
	   (p8)	sub		r19 = r16, r17, 1
	   (p9)	sub		r19 = r16, r17		;;
} { .mfi;	ld8		r16 = [r33], 8
		nop.f		0
	   (p8)	cmp.leu		p6, p7 = r20, r19
} { .mfi;	ld8		r17 = [r34], 8
		nop.f		0
	   (p9)	cmp.ltu		p6, p7 = r20, r19
} { .mbb;	st8		[r32] = r19, 8
		nop.b		0
		br.cloop.dptk	.Loopm			;;
}
	.pred.rel "mutex",p6,p7
  { .mfi;  (p6)	sub		r19 = r16, r17, 1
		nop.f		0
	   (p7)	sub		r19 = r16, r17		;;
} { .mii;	st8		[r32] = r19
	   (p6)	cmp.leu		p8, p9 = r16, r19
	   (p7)	cmp.ltu		p8, p9 = r16, r19	;;
}
	.pred.rel "mutex",p8,p9
  { .mfi;  (p8)	mov		r8 = 1
		nop.f		0
	   (p9)	mov		r8 = 0
}
		mov		ar.lc = r2
		br.ret.sptk.many b0
.Loopm:
	.pred.rel "mutex",p6,p7
  { .mii;	mov		r20 = r16
	   (p6)	sub		r19 = r16, r17, 1
	   (p7)	sub		r19 = r16, r17		;;
} { .mfi;	ld8		r16 = [r33], 8
		nop.f		0
	   (p6)	cmp.leu		p8, p9 = r20, r19
} { .mfi;	ld8		r17 = [r34], 8
		nop.f		0
	   (p7)	cmp.ltu		p8, p9 = r20, r19
} { .mbb;	st8		[r32] = r19, 8
		nop.b		0
		br.cloop.dptk	.Loop			;;
}
.Lend:
	.pred.rel "mutex",p8,p9
  { .mfi;  (p8)	sub		r19 = r16, r17, 1
		nop.f		0
	   (p9)	sub		r19 = r16, r17		;;
} { .mii;	st8		[r32] = r19
	   (p8)	cmp.leu		p6, p7 = r16, r19
	   (p9)	cmp.ltu		p6, p7 = r16, r19	;;
}
	.pred.rel "mutex",p6,p7
  { .mfi;  (p6)	mov		r8 = 1
		nop.f		0
	   (p7)	mov		r8 = 0
}
		mov		ar.lc = r2
		br.ret.sptk.many b0
EPILOGUE(mpn_sub_n)
ASM_END()
