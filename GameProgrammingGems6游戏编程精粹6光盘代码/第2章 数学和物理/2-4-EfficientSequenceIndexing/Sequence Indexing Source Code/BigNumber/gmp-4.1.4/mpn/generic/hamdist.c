/* mpn_hamdist -- mpn hamming distance.

Copyright 1994, 1996, 2000, 2001 Free Software Foundation, Inc.

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


unsigned long
mpn_hamdist (mp_srcptr up, mp_srcptr vp, mp_size_t size)
{
  unsigned long  result = 0;
  unsigned long  n;

  ASSERT (size >= 1);

  do {
    popc_limb (n, *up++ ^ *vp++);
    result += n;
  } while (--size != 0);

  return result;
}
