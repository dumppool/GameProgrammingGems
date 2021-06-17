/* mpfr_out_str -- output a floating-point number to a stream

Copyright 1999, 2001, 2003 Free Software Foundation, Inc.

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
#include <string.h>
#include <limits.h>
#include "gmp.h"
#include "gmp-impl.h"
#include "mpfr.h"
#include "mpfr-impl.h"

size_t 
mpfr_out_str (FILE *stream, int base, size_t n_digits, mpfr_srcptr op,
	      mp_rnd_t rnd_mode)
{
  char *s, *s0;
  size_t l;
  mp_exp_t e;

  /* when stream=NULL, output to stdout */
  if (stream == NULL)
    stream = stdout;

  if (MPFR_IS_NAN(op))
    {
      fprintf (stream, "NaN");
      return 3;
    }

  if (MPFR_IS_INF(op)) 
    { 
      if (MPFR_SIGN(op) > 0)
	{
	  fprintf (stream, "Inf");
	  return 3;
	}
      else
	{
	  fprintf (stream, "-Inf");
	  return 4;
	}
    }

  if (MPFR_IS_ZERO(op))
    {
      if (MPFR_SIGN(op) > 0)
        {
          fprintf(stream, "0");
          return 1;
        }
      else
        {
          fprintf(stream, "-0");
          return 2;
        }
    }

  s = mpfr_get_str (NULL, &e, base, n_digits, op, rnd_mode);

  s0 = s;
  /* for op=3.1416 we have s = "31416" and e = 1 */

  l = strlen (s) + 1; /* size of allocated block returned by mpfr_get_str
                         - may be incorrect, as only an upper bound? */
  if (*s == '-')
    fputc (*s++, stream);

  /* outputs mantissa */
  fputc (*s++, stream); e--; /* leading digit */
  fputc ('.', stream);       /* decimal point */
  fputs (s, stream);         /* rest of mantissa */
  (*__gmp_free_func) (s0, l);

  /* outputs exponent */
  if (e)
    {
      MPFR_ASSERTN(e >= LONG_MIN);
      MPFR_ASSERTN(e <= LONG_MAX);
      l += fprintf (stream, (base <= 10 ? "e%ld" : "@%ld"), (long) e);
    }

  return l;
}
