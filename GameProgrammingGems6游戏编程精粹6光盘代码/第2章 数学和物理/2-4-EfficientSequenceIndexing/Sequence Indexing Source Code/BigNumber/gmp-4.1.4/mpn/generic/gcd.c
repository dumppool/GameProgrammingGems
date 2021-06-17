/* mpn/gcd.c: mpn_gcd for gcd of two odd integers.

Copyright 1991, 1993, 1994, 1995, 1996, 1997, 1998, 2000, 2001, 2002 Free
Software Foundation, Inc.

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

/* Integer greatest common divisor of two unsigned integers, using
   the accelerated algorithm (see reference below).

   mp_size_t mpn_gcd (up, usize, vp, vsize).

   Preconditions [U = (up, usize) and V = (vp, vsize)]:

   1.  V is odd.
   2.  numbits(U) >= numbits(V).

   Both U and V are destroyed by the operation.  The result is left at vp,
   and its size is returned.

   Ken Weber (kweber@mat.ufrgs.br, kweber@mcs.kent.edu)

   Funding for this work has been partially provided by Conselho Nacional
   de Desenvolvimento Cienti'fico e Tecnolo'gico (CNPq) do Brazil, Grant
   301314194-2, and was done while I was a visiting reseacher in the Instituto
   de Matema'tica at Universidade Federal do Rio Grande do Sul (UFRGS).

   Refer to
	K. Weber, The accelerated integer GCD algorithm, ACM Transactions on
	Mathematical Software, v. 21 (March), 1995, pp. 111-122.  */

#include "gmp.h"
#include "gmp-impl.h"
#include "longlong.h"

/* If MIN (usize, vsize) >= GCD_ACCEL_THRESHOLD, then the accelerated
   algorithm is used, otherwise the binary algorithm is used.  This may be
   adjusted for different architectures.  */
#ifndef GCD_ACCEL_THRESHOLD
#define GCD_ACCEL_THRESHOLD 5
#endif

/* When U and V differ in size by more than BMOD_THRESHOLD, the accelerated
   algorithm reduces using the bmod operation.  Otherwise, the k-ary reduction
   is used.  0 <= BMOD_THRESHOLD < GMP_NUMB_BITS.  */
enum
  {
    BMOD_THRESHOLD = GMP_NUMB_BITS/2
  };


/* Use binary algorithm to compute V <-- GCD (V, U) for usize, vsize == 2.
   Both U and V must be odd.  */
static inline mp_size_t
gcd_2 (mp_ptr vp, mp_srcptr up)
{
  mp_limb_t u0, u1, v0, v1;
  mp_size_t vsize;

  u0 = up[0];
  u1 = up[1];
  v0 = vp[0];
  v1 = vp[1];

  while (u1 != v1 && u0 != v0)
    {
      unsigned long int r;
      if (u1 > v1)
	{
	  u1 -= v1 + (u0 < v0);
	  u0 = (u0 - v0) & GMP_NUMB_MASK;
	  count_trailing_zeros (r, u0);
	  u0 = ((u1 << (GMP_NUMB_BITS - r)) & GMP_NUMB_MASK) | (u0 >> r);
	  u1 >>= r;
	}
      else  /* u1 < v1.  */
	{
	  v1 -= u1 + (v0 < u0);
	  v0 = (v0 - u0) & GMP_NUMB_MASK;
	  count_trailing_zeros (r, v0);
	  v0 = ((v1 << (GMP_NUMB_BITS - r)) & GMP_NUMB_MASK) | (v0 >> r);
	  v1 >>= r;
	}
    }

  vp[0] = v0, vp[1] = v1, vsize = 1 + (v1 != 0);

  /* If U == V == GCD, done.  Otherwise, compute GCD (V, |U - V|).  */
  if (u1 == v1 && u0 == v0)
    return vsize;

  v0 = (u0 == v0) ? (u1 > v1) ? u1-v1 : v1-u1 : (u0 > v0) ? u0-v0 : v0-u0;
  vp[0] = mpn_gcd_1 (vp, vsize, v0);

  return 1;
}

/* The function find_a finds 0 < N < 2^GMP_NUMB_BITS such that there exists
   0 < |D| < 2^GMP_NUMB_BITS, and N == D * C mod 2^(2*GMP_NUMB_BITS).
   In the reference article, D was computed along with N, but it is better to
   compute D separately as D <-- N / C mod 2^(GMP_NUMB_BITS + 1), treating
   the result as a twos' complement signed integer.

   Initialize N1 to C mod 2^(2*GMP_NUMB_BITS).  According to the reference
   article, N2 should be initialized to 2^(2*GMP_NUMB_BITS), but we use
   2^(2*GMP_NUMB_BITS) - N1 to start the calculations within double
   precision.  If N2 > N1 initially, the first iteration of the while loop
   will swap them.  In all other situations, N1 >= N2 is maintained.  */

#if HAVE_NATIVE_mpn_gcd_finda
#define find_a(cp)  mpn_gcd_finda (cp)

#else
static
#if ! defined (__i386__)
inline				/* don't inline this for the x86 */
#endif
mp_limb_t
find_a (mp_srcptr cp)
{
  unsigned long int leading_zero_bits = 0;

  mp_limb_t n1_l = cp[0];	/* N1 == n1_h * 2^GMP_NUMB_BITS + n1_l.  */
  mp_limb_t n1_h = cp[1];

  mp_limb_t n2_l = (-n1_l & GMP_NUMB_MASK);	/* N2 == n2_h * 2^GMP_NUMB_BITS + n2_l.  */
  mp_limb_t n2_h = (~n1_h & GMP_NUMB_MASK);

  /* Main loop.  */
  while (n2_h != 0)		/* While N2 >= 2^GMP_NUMB_BITS.  */
    {
      /* N1 <-- N1 % N2.  */
      if (((GMP_NUMB_HIGHBIT >> leading_zero_bits) & n2_h) == 0)
	{
	  unsigned long int i;
	  count_leading_zeros (i, n2_h);
	  i -= GMP_NAIL_BITS;
	  i -= leading_zero_bits;
	  leading_zero_bits += i;
	  n2_h = ((n2_h << i) & GMP_NUMB_MASK) | (n2_l >> (GMP_NUMB_BITS - i));
	  n2_l = (n2_l << i) & GMP_NUMB_MASK;
	  do
	    {
	      if (n1_h > n2_h || (n1_h == n2_h && n1_l >= n2_l))
		{
		  n1_h -= n2_h + (n1_l < n2_l);
		  n1_l = (n1_l - n2_l) & GMP_NUMB_MASK;
		}
	      n2_l = (n2_l >> 1) | ((n2_h << (GMP_NUMB_BITS - 1)) & GMP_NUMB_MASK);
	      n2_h >>= 1;
	      i -= 1;
	    }
	  while (i != 0);
	}
      if (n1_h > n2_h || (n1_h == n2_h && n1_l >= n2_l))
	{
	  n1_h -= n2_h + (n1_l < n2_l);
	  n1_l = (n1_l - n2_l) & GMP_NUMB_MASK;
	}

      MP_LIMB_T_SWAP (n1_h, n2_h);
      MP_LIMB_T_SWAP (n1_l, n2_l);
    }

  return n2_l;
}
#endif


mp_size_t
mpn_gcd (mp_ptr gp, mp_ptr up, mp_size_t usize, mp_ptr vp, mp_size_t vsize)
{
  mp_ptr orig_vp = vp;
  mp_size_t orig_vsize = vsize;
  int binary_gcd_ctr;		/* Number of times binary gcd will execute.  */
  TMP_DECL (marker);

  ASSERT (usize >= 1);
  ASSERT (vsize >= 1);
  ASSERT (usize >= vsize);
  ASSERT (vp[0] & 1);
  ASSERT (up[usize - 1] != 0);
  ASSERT (vp[vsize - 1] != 0);
#if WANT_ASSERT
  if (usize == vsize)
    {
      int  uzeros, vzeros;
      count_leading_zeros (uzeros, up[usize - 1]);
      count_leading_zeros (vzeros, vp[vsize - 1]);
      ASSERT (uzeros <= vzeros);
    }
#endif
  ASSERT (! MPN_OVERLAP_P (up, usize, vp, vsize));
  ASSERT (MPN_SAME_OR_SEPARATE2_P (gp, vsize, up, usize));
  ASSERT (MPN_SAME_OR_SEPARATE2_P (gp, vsize, vp, vsize));

  TMP_MARK (marker);

  /* Use accelerated algorithm if vsize is over GCD_ACCEL_THRESHOLD.
     Two EXTRA limbs for U and V are required for kary reduction.  */
  if (vsize >= GCD_ACCEL_THRESHOLD)
    {
      unsigned long int vbitsize, d;
      mp_ptr orig_up = up;
      mp_size_t orig_usize = usize;
      mp_ptr anchor_up = (mp_ptr) TMP_ALLOC ((usize + 2) * BYTES_PER_MP_LIMB);

      MPN_COPY (anchor_up, orig_up, usize);
      up = anchor_up;

      count_leading_zeros (d, up[usize - 1]);
      d -= GMP_NAIL_BITS;
      d = usize * GMP_NUMB_BITS - d;
      count_leading_zeros (vbitsize, vp[vsize - 1]);
      vbitsize -= GMP_NAIL_BITS;
      vbitsize = vsize * GMP_NUMB_BITS - vbitsize;
      ASSERT (d >= vbitsize);
      d = d - vbitsize + 1;

      /* Use bmod reduction to quickly discover whether V divides U.  */
      up[usize++] = 0;				/* Insert leading zero.  */
      mpn_bdivmod (up, up, usize, vp, vsize, d);

      /* Now skip U/V mod 2^d and any low zero limbs.  */
      d /= GMP_NUMB_BITS, up += d, usize -= d;
      while (usize != 0 && up[0] == 0)
	up++, usize--;

      if (usize == 0)				/* GCD == ORIG_V.  */
	goto done;

      vp = (mp_ptr) TMP_ALLOC ((vsize + 2) * BYTES_PER_MP_LIMB);
      MPN_COPY (vp, orig_vp, vsize);

      do					/* Main loop.  */
	{
	  /* mpn_com_n can't be used here because anchor_up and up may
	     partially overlap */
	  if ((up[usize - 1] & GMP_NUMB_HIGHBIT) != 0)  /* U < 0; take twos' compl. */
	    {
	      mp_size_t i;
	      anchor_up[0] = -up[0] & GMP_NUMB_MASK;
	      for (i = 1; i < usize; i++)
		anchor_up[i] = (~up[i] & GMP_NUMB_MASK);
	      up = anchor_up;
	    }

	  MPN_NORMALIZE_NOT_ZERO (up, usize);

	  if ((up[0] & 1) == 0)			/* Result even; remove twos. */
	    {
	      unsigned int r;
	      count_trailing_zeros (r, up[0]);
	      mpn_rshift (anchor_up, up, usize, r);
	      usize -= (anchor_up[usize - 1] == 0);
	    }
	  else if (anchor_up != up)
	    MPN_COPY_INCR (anchor_up, up, usize);

	  MPN_PTR_SWAP (anchor_up,usize, vp,vsize);
	  up = anchor_up;

	  if (vsize <= 2)		/* Kary can't handle < 2 limbs and  */
	    break;			/* isn't efficient for == 2 limbs.  */

	  d = vbitsize;
	  count_leading_zeros (vbitsize, vp[vsize - 1]);
	  vbitsize -= GMP_NAIL_BITS;
	  vbitsize = vsize * GMP_NUMB_BITS - vbitsize;
	  d = d - vbitsize + 1;

	  if (d > BMOD_THRESHOLD)	/* Bmod reduction.  */
	    {
	      up[usize++] = 0;
	      mpn_bdivmod (up, up, usize, vp, vsize, d);
	      d /= GMP_NUMB_BITS, up += d, usize -= d;
	    }
	  else				/* Kary reduction.  */
	    {
	      mp_limb_t bp[2], cp[2];

	      /* C <-- V/U mod 2^(2*GMP_NUMB_BITS).  */
	      {
		mp_limb_t u_inv, hi, lo;
		modlimb_invert (u_inv, up[0]);
		cp[0] = (vp[0] * u_inv) & GMP_NUMB_MASK;
		umul_ppmm (hi, lo, cp[0], up[0] << GMP_NAIL_BITS);
		lo >>= GMP_NAIL_BITS;
		cp[1] = (vp[1] - hi - cp[0] * up[1]) * u_inv & GMP_NUMB_MASK;
	      }

	      /* U <-- find_a (C)  *  U.  */
	      up[usize] = mpn_mul_1 (up, up, usize, find_a (cp));
	      usize++;

	      /* B <-- A/C == U/V mod 2^(GMP_NUMB_BITS + 1).
		  bp[0] <-- U/V mod 2^GMP_NUMB_BITS and
		  bp[1] <-- ( (U - bp[0] * V)/2^GMP_NUMB_BITS ) / V mod 2

		  Like V/U above, but simplified because only the low bit of
		  bp[1] is wanted. */
	      {
		mp_limb_t  v_inv, hi, lo;
		modlimb_invert (v_inv, vp[0]);
		bp[0] = (up[0] * v_inv) & GMP_NUMB_MASK;
		umul_ppmm (hi, lo, bp[0], vp[0] << GMP_NAIL_BITS);
		lo >>= GMP_NAIL_BITS;
		bp[1] = (up[1] + hi + (bp[0] & vp[1])) & 1;
	      }

	      up[usize++] = 0;
	      if (bp[1] != 0)	/* B < 0: U <-- U + (-B)  * V.  */
		{
		   mp_limb_t c = mpn_addmul_1 (up, vp, vsize, -bp[0] & GMP_NUMB_MASK);
		   mpn_add_1 (up + vsize, up + vsize, usize - vsize, c);
		}
	      else		/* B >= 0:  U <-- U - B * V.  */
		{
		  mp_limb_t b = mpn_submul_1 (up, vp, vsize, bp[0]);
		  mpn_sub_1 (up + vsize, up + vsize, usize - vsize, b);
		}

	      up += 2, usize -= 2;  /* At least two low limbs are zero.  */
	    }

	  /* Must remove low zero limbs before complementing.  */
	  while (usize != 0 && up[0] == 0)
	    up++, usize--;
	}
      while (usize != 0);

      /* Compute GCD (ORIG_V, GCD (ORIG_U, V)).  Binary will execute twice.  */
      up = orig_up, usize = orig_usize;
      binary_gcd_ctr = 2;
    }
  else
    binary_gcd_ctr = 1;

  /* Finish up with the binary algorithm.  Executes once or twice.  */
  for ( ; binary_gcd_ctr--; up = orig_vp, usize = orig_vsize)
    {
      if (usize > 2)		/* First make U close to V in size.  */
	{
	  unsigned long int vbitsize, d;
	  count_leading_zeros (d, up[usize - 1]);
	  d -= GMP_NAIL_BITS;
	  d = usize * GMP_NUMB_BITS - d;
	  count_leading_zeros (vbitsize, vp[vsize - 1]);
	  vbitsize -= GMP_NAIL_BITS;
	  vbitsize = vsize * GMP_NUMB_BITS - vbitsize;
	  d = d - vbitsize - 1;
	  if (d != -(unsigned long int)1 && d > 2)
	    {
	      mpn_bdivmod (up, up, usize, vp, vsize, d);  /* Result > 0.  */
	      d /= (unsigned long int)GMP_NUMB_BITS, up += d, usize -= d;
	    }
	}

      /* Start binary GCD.  */
      do
	{
	  mp_size_t zeros;

	  /* Make sure U is odd.  */
	  MPN_NORMALIZE (up, usize);
	  while (up[0] == 0)
	    up += 1, usize -= 1;
	  if ((up[0] & 1) == 0)
	    {
	      unsigned int r;
	      count_trailing_zeros (r, up[0]);
	      mpn_rshift (up, up, usize, r);
	      usize -= (up[usize - 1] == 0);
	    }

	  /* Keep usize >= vsize.  */
	  if (usize < vsize)
	    MPN_PTR_SWAP (up, usize, vp, vsize);

	  if (usize <= 2)				/* Double precision. */
	    {
	      if (vsize == 1)
		vp[0] = mpn_gcd_1 (up, usize, vp[0]);
	      else
		vsize = gcd_2 (vp, up);
	      break;					/* Binary GCD done.  */
	    }

	  /* Count number of low zero limbs of U - V.  */
	  for (zeros = 0; up[zeros] == vp[zeros] && ++zeros != vsize; )
	    continue;

	  /* If U < V, swap U and V; in any case, subtract V from U.  */
	  if (zeros == vsize)				/* Subtract done.  */
	    up += zeros, usize -= zeros;
	  else if (usize == vsize)
	    {
	      mp_size_t size = vsize;
	      do
		size--;
	      while (up[size] == vp[size]);
	      if (up[size] < vp[size])			/* usize == vsize.  */
		MP_PTR_SWAP (up, vp);
	      up += zeros, usize = size + 1 - zeros;
	      mpn_sub_n (up, up, vp + zeros, usize);
	    }
	  else
	    {
	      mp_size_t size = vsize - zeros;
	      up += zeros, usize -= zeros;
	      if (mpn_sub_n (up, up, vp + zeros, size))
		{
		  while (up[size] == 0)			/* Propagate borrow. */
		    up[size++] = -(mp_limb_t)1;
		  up[size] -= 1;
		}
	    }
	}
      while (usize);					/* End binary GCD.  */
    }

done:
  if (vp != gp)
    MPN_COPY_INCR (gp, vp, vsize);
  TMP_FREE (marker);
  return vsize;
}
