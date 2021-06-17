/* mpz_out_raw -- write an mpz_t in raw format.

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
MA 02111-1307, USA. */

#include <stdio.h>
#include "gmp.h"
#include "gmp-impl.h"
#include "longlong.h"


/* HTON_LIMB_STORE takes a normal host byte order limb and stores it as
   network byte order (ie. big endian). */

#if HAVE_LIMB_BIG_ENDIAN
#define HTON_LIMB_STORE(dst, limb)  do { *(dst) = (limb); } while (0)
#endif

/* The generic implementations below very likely come out as lots of
   separate byte stores, so if we know the host is little endian then
   instead use a purely arithmetic BSWAP_LIMB and a single store.  */
#if HAVE_LIMB_LITTLE_ENDIAN
#define HTON_LIMB_STORE(dst, limb)  BSWAP_LIMB_STORE (dst, limb)
#endif

#if ! defined (HTON_LIMB_STORE)
#if BITS_PER_MP_LIMB == 8
#define HTON_LIMB_STORE(dst, limb)  do { *(dst) = (limb); } while (0)
#endif
#if BITS_PER_MP_LIMB == 16
#define HTON_LIMB_STORE(dst, limb)      \
  do {                                  \
    mp_limb_t  __limb = (limb);         \
    char  *__p = (char *) (dst);        \
    __p[1] = (__limb);                  \
    __p[0] = (__limb) >> 8;             \
  } while (0)
#endif
#if BITS_PER_MP_LIMB == 32
#define HTON_LIMB_STORE(dst, limb)      \
  do {                                  \
    mp_limb_t  __limb = (limb);         \
    char  *__p = (char *) (dst);        \
    __p[3] = (__limb);                  \
    __p[2] = (__limb) >> 8;             \
    __p[1] = (__limb) >> 16;            \
    __p[0] = (__limb) >> 24;            \
  } while (0)
#endif
#if BITS_PER_MP_LIMB == 64
#define HTON_LIMB_STORE(dst, limb)      \
  do {                                  \
    mp_limb_t  __limb = (limb);         \
    char  *__p = (char *) (dst);        \
    __p[7] = (__limb);                  \
    __p[6] = (__limb) >> 8;             \
    __p[5] = (__limb) >> 16;            \
    __p[4] = (__limb) >> 24;            \
    __p[3] = (__limb) >> 32;            \
    __p[2] = (__limb) >> 40;            \
    __p[1] = (__limb) >> 48;            \
    __p[0] = (__limb) >> 56;            \
  } while (0)
#endif
#endif


size_t
mpz_out_raw (FILE *fp, mpz_srcptr x)
{
  mp_size_t   xsize, abs_xsize, bytes, i;
  mp_srcptr   xp;
  char        *tp, *bp;
  mp_limb_t   xlimb;
  int         zeros;
  size_t      tsize, ssize;

  xsize = SIZ(x);
  abs_xsize = ABS (xsize);
  bytes = (abs_xsize * GMP_NUMB_BITS + 7) / 8;
  tsize = ROUND_UP_MULTIPLE ((unsigned) 4, BYTES_PER_MP_LIMB) + bytes;

  tp = __GMP_ALLOCATE_FUNC_TYPE (tsize, char);
  bp = tp + ROUND_UP_MULTIPLE ((unsigned) 4, BYTES_PER_MP_LIMB);

  if (bytes != 0)
    {
      bp += bytes;
      xp = PTR (x);
      i = abs_xsize;

      if (GMP_NAIL_BITS == 0)
        {
          /* reverse limb order, and byte swap if necessary */
#ifdef _CRAY
          _Pragma ("_CRI ivdep");
#endif
          do
            {
              bp -= BYTES_PER_MP_LIMB;
              xlimb = *xp;
              HTON_LIMB_STORE ((mp_ptr) bp, xlimb);
              xp++;
            }
          while (--i > 0);

          /* strip high zero bytes (without fetching from bp) */
          count_leading_zeros (zeros, xlimb);
          zeros /= 8;
          bp += zeros;
          bytes -= zeros;
        }
      else
        {
          mp_limb_t  new_xlimb;
          int        bits;
          ASSERT_CODE (char *bp_orig = bp - bytes);

          ASSERT_ALWAYS (GMP_NUMB_BITS >= 8);

          bits = 0;
          xlimb = 0;
          for (;;)
            {
              while (bits >= 8)
                {
                  ASSERT (bp > bp_orig);
                  *--bp = xlimb & 0xFF;
                  xlimb >>= 8;
                  bits -= 8;
                }

              if (i == 0)
                break;

              new_xlimb = *xp++;
              i--;
              ASSERT (bp > bp_orig);
              *--bp = (xlimb | (new_xlimb << bits)) & 0xFF;
              xlimb = new_xlimb >> (8 - bits);
              bits += GMP_NUMB_BITS - 8;
            }

          if (bits != 0)
            {
              ASSERT (bp > bp_orig);
              *--bp = xlimb;
            }

          ASSERT (bp == bp_orig);
          while (*bp == 0)
            {
              bp++;
              bytes--;
            }
        }
    }

  /* total bytes to be written */
  ssize = 4 + bytes;

  /* twos complement negative for the size value */
  bytes = (xsize >= 0 ? bytes : -bytes);

  /* so we don't rely on sign extension in ">>" */
  ASSERT_ALWAYS (sizeof (bytes) >= 4);

  bp[-4] = bytes >> 24;
  bp[-3] = bytes >> 16;
  bp[-2] = bytes >> 8;
  bp[-1] = bytes;
  bp -= 4;

  if (fp == 0)
    fp = stdout;
  if (fwrite (bp, ssize, 1, fp) != 1)
    ssize = 0;

  (*__gmp_free_func) (tp, tsize);
  return ssize;
}
