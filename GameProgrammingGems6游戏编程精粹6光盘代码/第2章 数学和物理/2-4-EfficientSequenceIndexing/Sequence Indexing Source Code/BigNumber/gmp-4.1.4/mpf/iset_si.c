/* mpf_init_set_si() -- Initialize a float and assign it from a signed int.

Copyright 1993, 1994, 1995, 2000, 2001 Free Software Foundation, Inc.

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
mpf_init_set_si (mpf_ptr r, long int val)
{
  mp_size_t prec = __gmp_default_fp_limb_precision;
  r->_mp_d = (mp_ptr) (*__gmp_allocate_func) ((prec + 1) * BYTES_PER_MP_LIMB);
  r->_mp_prec = prec;

  if (val > 0)
    {
      r->_mp_d[0] = val;
      r->_mp_size = 1;
      r->_mp_exp = 1;
    }
  else if (val < 0)
    {
      r->_mp_d[0] = (unsigned long) -val;
      r->_mp_size = -1;
      r->_mp_exp = 1;
    }
  else
    {
      r->_mp_size = 0;
      r->_mp_exp = 0;
    }
}
