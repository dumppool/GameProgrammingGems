/* mpz_ui_sub -- Subtract an unsigned one-word integer and an mpz_t.

Copyright 2002 Free Software Foundation, Inc.

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
mpz_ui_sub (mpz_ptr w, unsigned long int uval, mpz_srcptr v)
{
  mp_ptr vp, wp;
  mp_size_t vn, wn;
  mp_limb_t cy;

#if GMP_NAIL_BITS != 0
  if (uval > GMP_NUMB_MAX)
    {
      mpz_t u;
      mp_limb_t ul[2];
      PTR(u) = ul;
      ul[0] = uval & GMP_NUMB_MASK;
      ul[1] = uval >> GMP_NUMB_BITS;
      SIZ(u) = 2;
      mpz_sub (w, u, v);
      return;
    }
#endif

  vp = PTR(v);
  vn = SIZ(v);

  wp = PTR(w);

  if (vn > 1)
    {
      wp = MPZ_REALLOC (w, vn);
      vp = PTR(v);
      mpn_sub_1 (wp, vp, vn, (mp_limb_t) uval);
      wn = -(vn - (wp[vn - 1] == 0));
    }
  else if (vn == 1)
    {
      if (uval >= vp[0])
	{
	  wp[0] = uval - vp[0];
	  wn = wp[0] != 0;
	}
      else
	{
	  wp[0] = vp[0] - uval;
	  wn = -1;
	}
    }
  else if (vn == 0)
    {
      wp[0] = uval;
      wn = uval != 0;
    }
  else /* (vn < 0) */
    {
      vn = -vn;
      wp = MPZ_REALLOC (w, vn + 1);
      vp = PTR(v);
      cy = mpn_add_1 (wp, vp, vn, (mp_limb_t) uval);
      wp[vn] = cy;
      wn = vn + (cy != 0);
    }

  SIZ(w) = wn;
}
