/* mpfr_mul_2si -- multiply a floating-point number by a power of two

Copyright 1999, 2001, 2002 Free Software Foundation, Inc.

This file is part of the MPFR Library.

The MPFR Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or (at your
option) any later version.

The MPFR Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the MPFR Library; see the file COPYING.LIB.  If not, write to
the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
MA 02111-1307, USA. */

#include "gmp.h"
#include "gmp-impl.h"
#include "mpfr.h"
#include "mpfr-impl.h"


/* In the overflow test "n > emax - emin" ensures "emax-n" doesn't wrap
   around if n is big.  It used to done with "emax < MPFR_EMIN_MIN + n", but
   that provoked a compiler bug on mips gcc 2.95.3 in -O2 -mabi=n32.
   Similarly for the underflow test.  */

int
mpfr_mul_2si (mpfr_ptr y, mpfr_srcptr x, long int n, mp_rnd_t rnd_mode)
{
  int inexact;

  inexact = y != x ? mpfr_set (y, x, rnd_mode) : 0;

  if (MPFR_IS_FP(y) && MPFR_NOTZERO(y))
    {
      if (n > 0
          && ((unsigned long) n >
              (unsigned long) (mp_exp_unsigned_t) (__mpfr_emax - __mpfr_emin)
              || MPFR_EXP(y) > __mpfr_emax - n))
        return mpfr_set_overflow (y, rnd_mode, MPFR_SIGN(y));

      if (n < 0
          && ((unsigned long) -n >
              (unsigned long) (mp_exp_unsigned_t) (__mpfr_emax - __mpfr_emin)
              || MPFR_EXP(y) < __mpfr_emin - n))
        return mpfr_set_underflow (y, rnd_mode, MPFR_SIGN(y));

      MPFR_EXP(y) += n;
    }

  return inexact;
}
