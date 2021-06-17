/* mpz_divisible_p -- mpz by mpz divisibility test */

/*
Copyright 2001, 2002 Free Software Foundation, Inc.

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
MA 02111-1307, USA.
*/

#include "gmp.h"
#include "gmp-impl.h"

int
mpz_divisible_p (mpz_srcptr a, mpz_srcptr d)
{
  mp_size_t dsize;

  dsize = SIZ(d);
  if (dsize == 0)
    DIVIDE_BY_ZERO;

  return mpn_divisible_p (PTR(a), (mp_size_t) ABSIZ(a),
                          PTR(d), (mp_size_t) ABS(dsize));
}
