/* Test mtox.

Copyright 2002 Free Software Foundation, Inc.

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
#include "mp.h"
#include "tests.h"


void
check_random (void)
{
  mpz_t  z;
  int    i;
  char   *got, *want;
  gmp_randstate_ptr  rands = RANDS;

  mpz_init (z);

  for (i = 0; i < 1000; i++)
    {
      mpz_erandomb (z, rands, 6 * BITS_PER_MP_LIMB);
      got = mtox (z);
      want = mpz_get_str (NULL, 16, z);
      if (strcmp (got, want) != 0)
        {
          printf ("mtox wrong result\n");
          printf ("  got  \"%s\"\n", got);
          printf ("  want \"%s\"\n", want);
          abort ();
        }
      (*__gmp_free_func) (got, strlen (got) + 1);
      (*__gmp_free_func) (want, strlen (want) + 1);
    }

  mpz_clear (z);
}

void
check_mem (void)
{
  MINT  *m;
  char  *s;

  m = itom (0);
  s = mtox (m);
  if (! tests_memory_valid (s))
    {
      printf ("Skipping t-mtox, cannot test libgmp and libmp memory together\n");
      exit (0);
    }
  mfree (m);
  (*__gmp_free_func) (s, strlen (s) + 1);
}


int
main (void)
{
  tests_start ();

  check_mem ();
  check_random ();

  tests_end ();
  exit (0);
}
