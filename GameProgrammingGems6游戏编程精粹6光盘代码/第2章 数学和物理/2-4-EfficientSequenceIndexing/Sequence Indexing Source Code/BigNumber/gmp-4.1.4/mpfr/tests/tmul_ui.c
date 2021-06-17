/* Test file for mpfr_mul_ui.

Copyright 1999, 2000, 2001, 2002 Free Software Foundation.

This file is part of the MPFR Library.

The MPFR Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or (at your
option) any later version.

The MPFR Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the MPFR Library; see the file COPYING.LIB.  If not, write to
the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
MA 02111-1307, USA. */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "gmp.h"
#include "gmp-impl.h"
#include "mpfr.h"
#include "mpfr-impl.h"
#include "mpfr-test.h"

void check_inexact _PROTO((mp_prec_t));

void
check_inexact (mp_prec_t p)
{
  mpfr_t x, y, z;
  unsigned long u;
  mp_prec_t q;
  int inexact, cmp;
  mp_rnd_t rnd;

  mpfr_init2 (x, p);
  mpfr_init (y);
  mpfr_init2 (z, p + mp_bits_per_limb);
  mpfr_random (x);
  u = LONG_RAND();
  if (mpfr_mul_ui (z, x, u, GMP_RNDN))
    {
      fprintf (stderr, "Error: result should be exact\n");
      exit (1);
    }

  for (q=2; q<=p; q++)
    for (rnd=0; rnd<4; rnd++)
      {
	mpfr_set_prec (y, q);
	inexact = mpfr_mul_ui (y, x, u, rnd);
	cmp = mpfr_cmp (y, z);
	if (((inexact == 0) && (cmp != 0)) ||
	    ((inexact < 0) && (cmp >= 0)) ||
	    ((inexact > 0) && (cmp <= 0)))
	  {
	    fprintf (stderr, "Wrong inexact flag for p=%u, q=%u, rnd=%s\n",
		     (unsigned) p, (unsigned) q, mpfr_print_rnd_mode (rnd));
	    exit (1);
	  }
      }

  mpfr_set_prec (x, 2);
  mpfr_set_ui (x, 1, GMP_RNDN);
  if (mpfr_mul_ui (x, x, 5, GMP_RNDZ) == 0)
    {
      fprintf (stderr, "mul_ui(1, 5) cannot be exact with prec=2\n");
      exit (1);
    }

  mpfr_clear (x);
  mpfr_clear (y);
  mpfr_clear (z);
}

int
main (int argc, char *argv[])
{
  mpfr_t x, y;
  unsigned int xprec, yprec, i;
  mp_prec_t p;

  for (p=2; p<100; p++)
    for (i=1; i<50; i++)
      check_inexact (p);
  
  mpfr_init2 (x, 53);
  mpfr_init2 (y, 53);

  /* checks that result is normalized */
  mpfr_set_d (y, 6.93147180559945286227e-01, GMP_RNDZ);
  mpfr_mul_ui (x, y, 1, GMP_RNDZ);
  if (MPFR_MANT(x)[MPFR_PREC(x)/mp_bits_per_limb] >> (mp_bits_per_limb-1) == 0)
    {
      fprintf (stderr, "Error in mpfr_mul_ui: result not normalized\n");
      exit (1);
    }
  if (mpfr_cmp (x, y))
    {
      fprintf (stderr, "Error in mpfr_mul_ui: 1*y != y\n");
      printf ("y=  "); mpfr_print_binary (y); putchar ('\n');
      printf ("1*y="); mpfr_print_binary (x); putchar ('\n');
      exit (1);
  }


  mpfr_set_inf (x, 1);
  mpfr_mul_ui (x, x, 3, GMP_RNDU);
  if (!mpfr_inf_p (x) || (mpfr_sgn (x) <= 0))
    {
      fprintf (stderr, "Error in mpfr_mul_ui: +Inf*3 does not give +Inf\n");
      exit (1);
    }

  mpfr_set_inf (x, -1);
  mpfr_mul_ui (x, x, 3, GMP_RNDU);
  if (!mpfr_inf_p (x) || (mpfr_sgn (x) >= 0))
    {
      fprintf (stderr, "Error in mpfr_mul_ui: -Inf*3 does not give -Inf\n");
      exit (1);
    }

  mpfr_set_nan (x);
  mpfr_mul_ui (x, x, 3, GMP_RNDU);
  if (!mpfr_nan_p(x))
    {
      fprintf (stderr, "Error in mpfr_mul_ui: NaN*3 does not give NaN\n");
      exit (1);
    }

  mpfr_set_d (x, 1.0/3.0, GMP_RNDZ);
  mpfr_mul_ui (x, x, 3, GMP_RNDU);
  if (mpfr_get_d1 (x) != 1.0)
    {
      fprintf (stderr, "Error in mpfr_mul_ui: U(Z(1/3)*3) does not give 1\n");
      exit (1);
    }

  /* checks sign is correct */
  mpfr_set_d(x, -2.0, GMP_RNDZ);
  mpfr_set_d(y, 3.0, GMP_RNDZ);
  mpfr_mul_ui(x, y, 4, GMP_RNDZ);
  if (mpfr_cmp_ui(x, 0) <= 0) {
    fprintf(stderr, "Error in mpfr_mul_ui: 4*3.0 does not give a positive result:\n"); 
    mpfr_print_binary(x); putchar('\n');
    printf("mpfr_cmp_ui(x, 0) = %d\n", mpfr_cmp_ui(x, 0));
    exit(1);
  }

  mpfr_set_prec (x, 9);
  mpfr_set_prec (y, 9);
  mpfr_set_str_raw (y, "0.100001111E9"); /* 271 */
  mpfr_mul_ui (x, y, 1335, GMP_RNDN);
  mpfr_set_str_raw (y, "0.101100001E19"); /* 361472 */
  if (mpfr_cmp (x, y))
    {
      fprintf (stderr, "Error in mul_ui for 1335*(0.100001111E9)\n");
      printf ("got "); mpfr_print_binary (x); putchar ('\n');
      exit(1);
    }

  mpfr_set_prec(y, 100);
  mpfr_set_prec(x, 100);
  /* y = 1199781142214086656 */
  mpfr_set_str_raw(y, "0.1000010100110011110101001011110010101111000100001E61");
  mpfr_mul_ui(x, y, 121, GMP_RNDD);
  /* 121*y = 145173518207904485376, representable exactly */
  mpfr_set_str_raw(y, "0.1111101111010101111111100011010010111010111110110011001E67");
  if (mpfr_cmp(x, y)) {
    printf("Error for 121*y: expected result is:\n");
    mpfr_print_binary(y); putchar('\n');
  }

  mpfr_set_prec (x, 32);
  mpfr_set_str_raw (x, "0.10000000000000000000000000000000E1");
  mpfr_set_prec (y, 93);
  mpfr_mul_ui (y, x, 1, GMP_RNDN);

  mpfr_set_prec (x, 287);
  mpfr_set_str_raw (x, "0.1111E7");
  mpfr_set_prec (y, 289);
  mpfr_mul_ui (y, x, 6, GMP_RNDN);
  mpfr_set_str_raw (x, "0.101101E10");
  if (mpfr_cmp (x, y))
    {
      printf ("Error for 6 * 120\n");
      exit (1);
    }
  
  mpfr_set_prec (x, 68);
  mpfr_set_prec (y, 64);
  mpfr_set_d (x, 2143861251406875.0, GMP_RNDN);
  mpfr_mul_ui (y, x, 23, GMP_RNDN);
  mpfr_set_str_raw (x, "10101111001011100001100110101111110001010010011001101101.0");
  if (mpfr_cmp (x, y))
    {
      printf ("Error for 23 * 2143861251406875.0\n");
      printf ("expected "); mpfr_print_binary (x); putchar ('\n');
      printf ("got      "); mpfr_print_binary (y); putchar ('\n');
      exit (1);
    }
  

  for (xprec = 53; xprec <= 128; xprec++)
    {
      mpfr_set_prec (x, xprec);
      mpfr_set_str_raw (x, "0.1100100100001111110011111000000011011100001100110111E2");
      for (yprec = 53; yprec <= 128; yprec++)
	{
	  mpfr_set_prec (y, yprec);
	  mpfr_mul_ui (y, x, 1, GMP_RNDN);
	  if (mpfr_get_d1 (x) != mpfr_get_d1 (y))
	    {
	      fprintf (stderr, "multiplication by 1.0 fails for xprec=%u, yprec=%u\n", xprec, yprec);
	      printf ("expected "); mpfr_print_binary (x); putchar ('\n');
	      printf ("got      "); mpfr_print_binary (y); putchar ('\n');
	      exit (1);
	    }
	}
    }

  mpfr_set_prec (x, 128);
  mpfr_set_ui (x, 17, GMP_RNDN);
  mpfr_mul_ui (x, x, ULONG_HIGHBIT, GMP_RNDN);
  mpfr_set_prec (y, 128);
  mpfr_set_ui (y, ULONG_HIGHBIT, GMP_RNDN);
  mpfr_mul_ui (y, y, 17, GMP_RNDN);
  if (mpfr_cmp (x, y))
    {
      printf ("Error for 17 * ULONG_HIGHBIT\n");
      exit (1);
    }
  

  mpfr_clear(x); mpfr_clear(y);

  return 0;
}
