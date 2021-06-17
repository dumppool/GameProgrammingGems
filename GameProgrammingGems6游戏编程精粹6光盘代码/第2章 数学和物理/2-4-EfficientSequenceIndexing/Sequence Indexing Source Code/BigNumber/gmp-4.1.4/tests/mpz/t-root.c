/* Test mpz_add, mpz_add_ui, mpz_cmp, mpz_cmp, mpz_mul, mpz_sqrtrem.

Copyright 1991, 1993, 1994, 1996, 2000, 2001, 2002 Free Software Foundation, Inc.

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
#include <stdlib.h>

#include "gmp.h"
#include "gmp-impl.h"
#include "tests.h"

void debug_mp _PROTO ((mpz_t, int));

int
main (int argc, char **argv)
{
  mpz_t x2;
  mpz_t x;
  mpz_t temp, temp2;
  mp_size_t x2_size;
  int i;
  int reps = 1000;
  unsigned long nth;
  gmp_randstate_ptr rands;
  mpz_t bs;
  unsigned long bsi, size_range;

  tests_start ();
  rands = RANDS;

  mpz_init (bs);

  if (argc == 2)
     reps = atoi (argv[1]);

  mpz_init (x2);
  mpz_init (x);
  mpz_init (temp);
  mpz_init (temp2);

  for (i = 0; i < reps; i++)
    {
      mpz_urandomb (bs, rands, 32);
      size_range = mpz_get_ui (bs) % 12 + 2;

      mpz_urandomb (bs, rands, size_range);
      x2_size = mpz_get_ui (bs) + 10;
      mpz_rrandomb (x2, rands, x2_size);

      mpz_urandomb (bs, rands, 15);
      nth = mpz_getlimbn (bs, 0) % mpz_sizeinbase (x2, 2) + 2;

      mpz_root (x, x2, nth);

      mpz_urandomb (bs, rands, 4);
      bsi = mpz_get_ui (bs);
      if ((bsi & 1) != 0)
	{
	  /* With 50% probability, set x2 near a perfect power.  */
	  mpz_pow_ui (x2, x, nth);
	  if ((bsi & 2) != 0)
	    {
	      mpz_sub_ui (x2, x2, bsi >> 2);
	      mpz_abs (x2, x2);
	    }
	  else
	    mpz_add_ui (x2, x2, bsi >> 2);
	  mpz_root (x, x2, nth);
	}

      /* printf ("%ld %lu\n", SIZ (x2), nth); */

      mpz_pow_ui (temp, x, nth);

      /* Is power of result > argument?  */
      if (mpz_cmp (temp, x2) > 0)
	{
	  fprintf (stderr, "ERROR after test %d\n", i);
	  debug_mp (x2, 10);
	  debug_mp (x, 10);
	  fprintf (stderr, "nth: %lu\n", nth);
	  abort ();
	}

      if (nth > 1 && mpz_cmp_ui (temp, 1L) > 0 && ! mpz_perfect_power_p (temp))
	{
	  fprintf (stderr, "ERROR in mpz_perfect_power_p after test %d\n", i);
	  debug_mp (temp, 10);
	  debug_mp (x, 10);
	  fprintf (stderr, "nth: %lu\n", nth);
	  abort ();
	}

      if (nth > 10000)
	continue;		/* skip too expensive test */

      mpz_add_ui (temp2, x, 1L);
      mpz_pow_ui (temp2, temp2, nth);

      /* Is square of (result + 1) <= argument?  */
      if (mpz_cmp (temp2, x2) <= 0)
	{
	  fprintf (stderr, "ERROR after test %d\n", i);
	  debug_mp (x2, 10);
	  debug_mp (x, 10);
	  fprintf (stderr, "nth: %lu\n", nth);
	  abort ();
	}
    }

  mpz_clear (bs);
  mpz_clear (x2);
  mpz_clear (x);
  mpz_clear (temp);
  mpz_clear (temp2);

  tests_end ();
  exit (0);
}

void
debug_mp (mpz_t x, int base)
{
  mpz_out_str (stderr, base, x); fputc ('\n', stderr);
}
