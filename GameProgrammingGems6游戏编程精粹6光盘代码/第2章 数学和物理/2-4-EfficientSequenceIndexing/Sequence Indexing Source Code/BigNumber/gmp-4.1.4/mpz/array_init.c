/* mpz_array_init (array, array_size, size_per_elem) --

Copyright 1991, 1993, 1994, 1995, 2000, 2001, 2002 Free Software Foundation,
Inc.

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
mpz_array_init (mpz_ptr arr, mp_size_t arr_size, mp_size_t nbits)
{
  register mp_ptr p;
  register mp_size_t i;
  mp_size_t nlimbs;

  nlimbs = (nbits + GMP_NUMB_BITS - 1) / GMP_NUMB_BITS;
  p = (mp_ptr) (*__gmp_allocate_func) (arr_size * nlimbs * BYTES_PER_MP_LIMB);

  for (i = 0; i < arr_size; i++)
    {
      arr[i]._mp_alloc = nlimbs + 1; /* Yes, lie a little... */
      arr[i]._mp_size = 0;
      arr[i]._mp_d = p + i * nlimbs;
    }
}
