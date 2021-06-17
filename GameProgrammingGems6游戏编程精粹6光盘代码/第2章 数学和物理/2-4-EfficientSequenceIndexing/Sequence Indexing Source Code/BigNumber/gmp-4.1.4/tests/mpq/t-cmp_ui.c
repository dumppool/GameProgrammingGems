/* Test mpq_cmp_ui.

Copyright 1996, 1997, 2001 Free Software Foundation, Inc.

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

#define NUM(x) (&((x)->_mp_num))
#define DEN(x) (&((x)->_mp_den))

#define SGN(x) ((x) < 0 ? -1 : (x) > 0 ? 1 : 0)

int
ref_mpq_cmp_ui (mpq_t a, unsigned long int bn, unsigned long int bd)
{
  mpz_t ai, bi;
  int cc;

  mpz_init (ai);
  mpz_init (bi);

  mpz_mul_ui (ai, NUM (a), bd);
  mpz_mul_ui (bi, DEN (a), bn);
  cc = mpz_cmp (ai, bi);
  mpz_clear (ai);
  mpz_clear (bi);
  return cc;
}

#ifndef SIZE
#define SIZE 8	/* increasing this lowers the probabilty of finding an error */
#endif

int
main (int argc, char **argv)
{
  mpq_t a, b;
  mp_size_t size;
  int reps = 100000;
  int i;
  int cc, ccref;
  unsigned long int bn, bd;

  tests_start ();

  if (argc == 2)
     reps = atoi (argv[1]);

  mpq_init (a);
  mpq_init (b);

  for (i = 0; i < reps; i++)
    {
      size = urandom () % SIZE - SIZE/2;
      mpz_random2 (NUM (a), size);
      do
	{
	  size = urandom () % SIZE - SIZE/2;
	  mpz_random2 (DEN (a), size);
	}
      while (mpz_cmp_ui (DEN (a), 0) == 0);

      mpz_random2 (NUM (b), 1);
      mpz_mod_ui (NUM (b), NUM (b), ~(unsigned long int) 0);
      mpz_add_ui (NUM (b), NUM (b), 1);

      mpz_random2 (DEN (b), 1);
      mpz_mod_ui (DEN (b), DEN (b), ~(unsigned long int) 0);
      mpz_add_ui (DEN (b), DEN (b), 1);

      mpq_canonicalize (a);
      mpq_canonicalize (b);

      bn = mpz_get_ui (NUM (b));
      bd = mpz_get_ui (DEN (b));

      ccref = ref_mpq_cmp_ui (a, bn, bd);
      cc = mpq_cmp_ui (a, bn, bd);

      if (SGN (ccref) != SGN (cc))
	abort ();
    }

  mpq_clear (a);
  mpq_clear (b);

  tests_end ();
  exit (0);
}
