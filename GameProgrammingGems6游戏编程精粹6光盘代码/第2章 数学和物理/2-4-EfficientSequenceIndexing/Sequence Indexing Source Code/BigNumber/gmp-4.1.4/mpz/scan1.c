/* mpz_scan1 -- search for a 1 bit.

Copyright 2000, 2001, 2002 Free Software Foundation, Inc.

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
#include "longlong.h"


/* mpn_scan0 can't be used for the inverted u<0 search since there might not
   be a 0 bit before the end of the data.  mpn_scan1 could be used under u>0
   (except when in the high limb), but usually the search won't go very far
   so it seems reasonable to inline that code.  */

unsigned long
mpz_scan1 (mpz_srcptr u, unsigned long starting_bit)
{
  mp_srcptr      u_ptr = PTR(u);
  mp_size_t      size = SIZ(u);
  mp_size_t      abs_size = ABS(size);
  mp_srcptr      u_end = u_ptr + abs_size;
  unsigned long  starting_limb = starting_bit / GMP_NUMB_BITS;
  mp_srcptr      p = u_ptr + starting_limb;
  mp_limb_t      limb;
  int            cnt;

  /* Past the end there's no 1 bits for u>=0, or an immediate 1 bit for u<0.
     Notice this test picks up any u==0 too. */
  if (starting_limb >= abs_size)
    return (size >= 0 ? ULONG_MAX : starting_bit);

  limb = *p;

  if (size >= 0)
    {
      /* Mask to 0 all bits before starting_bit, thus ignoring them. */
      limb &= (MP_LIMB_T_MAX << (starting_bit % GMP_NUMB_BITS));

      if (limb == 0)
        {
          /* If it's the high limb which is zero after masking, then there's
             no 1 bits after starting_bit.  */
          p++;
          if (p == u_end)
            return ULONG_MAX;

          /* Otherwise search further for a non-zero limb.  The high limb is
             non-zero, if nothing else.  */
          for (;;)
            {
              limb = *p;
              if (limb != 0)
                break;
              p++;
              ASSERT (p < u_end);
            }
        }
    }
  else
    {
      mp_srcptr  q;

      /* If there's a non-zero limb before ours then we're in the ones
         complement region.  Search from *(p-1) downwards since that might
         give better cache locality, and since a non-zero in the middle of a
         number is perhaps a touch more likely than at the end.  */
      q = p;
      while (q != u_ptr)
        {
          q--;
          if (*q != 0)
            goto inverted;
        }

      if (limb == 0)
        {
          /* Skip zero limbs, to find the start of twos complement.  The
             high limb is non-zero, if nothing else.  This search is
             necessary so the -limb is applied at the right spot. */
          do
            {
              p++;
              ASSERT (p < u_end);
              limb = *p;
            }
          while (limb == 0);

          /* Apply twos complement, and look for a 1 bit in that.  Since
             limb!=0 here, also have (-limb)!=0 so there's certainly a 1
             bit.  */
          limb = -limb;
          goto got_limb;
        }

      /* Adjust so ~limb implied by searching for 0 bit becomes -limb.  */
      limb--;

    inverted:
      /* Now seeking a 0 bit. */

      /* Mask to 1 all bits before starting_bit, thus ignoring them. */
      limb |= (CNST_LIMB(1) << (starting_bit % GMP_NUMB_BITS)) - 1;

      /* Search for a limb which is not all ones.  If the end is reached
         then the zero immediately past the end is the result.  */
      while (limb == GMP_NUMB_MAX)
        {
          p++;
          if (p == u_end)
            return abs_size * GMP_NUMB_BITS;
          limb = *p;
        }

      /* Now seeking low 1 bit. */
      limb = ~limb;
    }

 got_limb:
  /* Mask to 0 all bits above the lowest 1 bit. */
  ASSERT (limb != 0);
  limb &= -limb;

  count_leading_zeros (cnt, limb);
  return (p - u_ptr) * GMP_NUMB_BITS + GMP_LIMB_BITS - 1 - cnt;
}
