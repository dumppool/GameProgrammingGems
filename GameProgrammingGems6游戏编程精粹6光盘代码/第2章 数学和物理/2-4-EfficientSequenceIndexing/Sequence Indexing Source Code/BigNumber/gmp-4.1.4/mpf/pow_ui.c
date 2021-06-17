/* mpf_pow_ui -- Compute b^e.

Copyright 1998, 1999, 2001 Free Software Foundation, Inc.

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
mpf_pow_ui (mpf_ptr r, mpf_srcptr b, unsigned long int e)
{
  mpf_t b2;
  unsigned long int e2;

  mpf_init2 (b2, mpf_get_prec (r));
  mpf_set (b2, b);
  mpf_set_ui (r, 1);

  if ((e & 1) != 0)
    mpf_set (r, b2);
  for (e2 = e >> 1; e2 != 0; e2 >>= 1)
    {
      mpf_mul (b2, b2, b2);
      if ((e2 & 1) != 0)
	mpf_mul (r, r, b2);
    }

  mpf_clear (b2);
}
