/* rpow -- MINT raised to short. */

/*
Copyright 2001 Free Software Foundation, Inc.

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

#include "mp.h"
#include "gmp.h"
#include "gmp-impl.h"

void
rpow (const MINT *b, short e, MINT *r)
{
  if (e >= 0)
    mpz_n_pow_ui (r, PTR(b), (mp_size_t) SIZ(b), (unsigned long) e);
  else
    SIZ(r) = 0;
}
