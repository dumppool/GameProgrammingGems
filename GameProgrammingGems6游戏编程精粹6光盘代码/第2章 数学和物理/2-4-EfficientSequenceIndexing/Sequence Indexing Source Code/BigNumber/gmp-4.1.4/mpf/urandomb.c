/* mpf_urandomb (rop, state, nbits) -- Generate a uniform pseudorandom
   real number between 0 (inclusive) and 1 (exclusive) of size NBITS,
   using STATE as the random state previously initialized by a call to
   gmp_randinit().

Copyright 1999, 2000, 2001, 2002  Free Software Foundation, Inc.

This file is part of the GNU MP Library.

The GNU MP Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or (at your
option) any later version.

The GNU MP Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the GNU MP Library; see the file COPYING.LIB.  If not, write to
the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
MA 02111-1307, USA. */

#include "gmp.h"
#include "gmp-impl.h"

void
mpf_urandomb (mpf_t rop, gmp_randstate_t rstate, unsigned long int nbits)
{
  mp_ptr rp;
  mp_size_t nlimbs;
  mp_exp_t exp;

  nbits = MIN (nbits, __GMPF_PREC_TO_BITS (rop->_mp_prec));

  rp = PTR (rop);
  nlimbs = (nbits + GMP_NUMB_BITS - 1) / GMP_NUMB_BITS;

  _gmp_rand (rp, rstate, nbits);

  /* If nbits isn't a multiple of GMP_NUMB_BITS, shift up.  */
  if (nlimbs != 0)
    {
      if (nbits % GMP_NUMB_BITS != 0)
	mpn_lshift (rp, rp, nlimbs, GMP_NUMB_BITS - nbits % GMP_NUMB_BITS);
    }

  exp = 0;
  while (nlimbs != 0 && rp[nlimbs - 1] == 0)
    {
      nlimbs--;
      exp--;
    }
  EXP (rop) = exp;
  SIZ (rop) = nlimbs;
}
