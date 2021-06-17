/* Test file for mpfr_asin.

Copyright 2001, 2002 Free Software Foundation.
Original version by Mathieu Dutour.

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
#include "gmp.h"
#include "mpfr.h"
#include "mpfr-test.h"

int
main (void)
{
  unsigned int prec, err, yprec, n;
  mp_rnd_t rnd;
  mpfr_t x, y, z;

  mpfr_init (x);
  mpfr_init (y);
  mpfr_init (z);

  /* check that sin(-1) = -Pi/2 */
  mpfr_set_si (x, -1, GMP_RNDN);
  mpfr_asin (y, x, GMP_RNDN);
  mpfr_const_pi (z, GMP_RNDN);
  mpfr_div_2exp (z, z, 1, GMP_RNDN);
  mpfr_neg (z, z, GMP_RNDN);
  if (mpfr_cmp (y, z))
    {
      fprintf (stderr, "sin(-1) is wrong, expected %.20e, got %.20e\n",
               mpfr_get_d1 (z), mpfr_get_d1 (y));
      exit (1);
    }

  for (prec = 2; prec <= 100; prec++)
    {
      mpfr_set_prec (x, prec);
      mpfr_set_prec (z, prec);
      yprec = prec + 10;

      for (n = 0; n < 10; n++)
	{
	  mpfr_random (x);
	  rnd = random () % 4;
	  mpfr_set_prec (y, yprec);
	  mpfr_asin (y, x, rnd);
	  err = (rnd == GMP_RNDN) ? yprec + 1 : yprec;
	  if (mpfr_can_round (y, err, rnd, rnd, prec))
	    {
	      mpfr_round_prec (y, rnd, prec);
	      mpfr_asin (z, x, rnd);
	      if (mpfr_cmp (y, z))
		{
		  printf ("results differ for x=");
		  mpfr_out_str (stdout, 2, prec, x, GMP_RNDN);
		  printf (" prec=%u rnd_mode=%s\n", prec,
			  mpfr_print_rnd_mode (rnd));
		  printf ("   got ");
		  mpfr_out_str (stdout, 2, prec, z, GMP_RNDN);
		  putchar ('\n');
		  printf ("   expected ");
		  mpfr_out_str (stdout, 2, prec, y, GMP_RNDN);
		  putchar ('\n');
		}
	    }
	}
    }

  mpfr_clear (x);
  mpfr_clear (y);
  mpfr_clear (z);

  return 0;
}
