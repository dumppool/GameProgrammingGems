/* Speed measuring program.

Copyright 1999, 2000, 2001, 2002 Free Software Foundation, Inc.

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

/* Usage message is in the code below, run with no arguments to print it.
   See README for interesting applications.

   To add a new routine foo(), create a speed_foo() function in the style of
   the existing ones and add an entry in the routine[] array.  Put FLAG_R if
   speed_foo() wants an "r" parameter.

   The routines don't have help messages or descriptions, but most have
   suggestive names.  See the source code for full details.

*/

#include "config.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if HAVE_UNISTD_H
#include <unistd.h>  /* for getpid, R_OK */
#endif

#if TIME_WITH_SYS_TIME
# include <sys/time.h>  /* for struct timeval */
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif

#if HAVE_SYS_RESOURCE_H
#include <sys/resource.h>  /* for getrusage() */
#endif


#include "gmp.h"
#include "gmp-impl.h"
#include "longlong.h"  /* for the benefit of speed-many.c */
#include "tests.h"
#include "speed.h"


#if !HAVE_DECL_OPTARG
extern char *optarg;
extern int optind, opterr;
#endif

#if !HAVE_STRTOUL
#define strtoul(p,e,b)  (unsigned long) strtol(p,e,b)
#endif

#ifdef SPEED_EXTRA_PROTOS
SPEED_EXTRA_PROTOS
#endif
#ifdef SPEED_EXTRA_PROTOS2
SPEED_EXTRA_PROTOS2
#endif


#define MPN_FILL(ptr, size, n)          \
  do {                                  \
    mp_size_t __i;                      \
    ASSERT ((size) >= 0);               \
    for (__i = 0; __i < (size); __i++)  \
      (ptr)[__i] = (n);                 \
  } while (0)


#if BITS_PER_MP_LIMB == 32
#define GMP_NUMB_0xAA  (CNST_LIMB(0xAAAAAAAA) & GMP_NUMB_MASK)
#endif
#if BITS_PER_MP_LIMB == 64
#define GMP_NUMB_0xAA  (CNST_LIMB(0xAAAAAAAAAAAAAAAA) & GMP_NUMB_MASK)
#endif


#define CMP_ABSOLUTE     1
#define CMP_RATIO        2
#define CMP_DIFFERENCE   3
#define CMP_DIFFPREV     4
int  option_cmp = CMP_ABSOLUTE;

#define UNIT_SECONDS        1
#define UNIT_CYCLES         2
#define UNIT_CYCLESPERLIMB  3
int  option_unit = UNIT_SECONDS;

#define DATA_RANDOM   1
#define DATA_RANDOM2  2
#define DATA_ZEROS    3
#define DATA_AAS      4
#define DATA_FFS      5
#define DATA_2FD      6
int  option_data = DATA_RANDOM;

int        option_square = 0;
double     option_factor = 0.0;
mp_size_t  option_step = 1;
int        option_gnuplot = 0;
char      *option_gnuplot_basename;
struct size_array_t {
  mp_size_t start, end;
} *size_array = NULL;
mp_size_t  size_num = 0;
mp_size_t  size_allocnum = 0;
int        option_resource_usage = 0;
long       option_seed = 123456789;

struct speed_params  sp;

#define COLUMN_WIDTH  13  /* for the free-form output */

#define FLAG_R            (1<<0)  /* require ".r" */
#define FLAG_R_OPTIONAL   (1<<1)  /* optional ".r" */
#define FLAG_RSIZE        (1<<2)
#define FLAG_NODATA       (1<<3)  /* don't alloc xp, yp */

const struct routine_t {
  /* constants */
  const char        *name;
  speed_function_t  fun;
  int               flag;
  
} routine[] = {

  { "noop",              speed_noop                 },
  { "noop_wxs",          speed_noop_wxs             },
  { "noop_wxys",         speed_noop_wxys            },

  { "mpn_add_n",         speed_mpn_add_n,     FLAG_R_OPTIONAL },
  { "mpn_sub_n",         speed_mpn_sub_n,     FLAG_R_OPTIONAL },

  { "mpn_addmul_1",      speed_mpn_addmul_1,  FLAG_R },
  { "mpn_submul_1",      speed_mpn_submul_1,  FLAG_R },
  { "mpn_mul_1",         speed_mpn_mul_1,     FLAG_R },
  { "mpn_mul_1_inplace", speed_mpn_mul_1_inplace, FLAG_R },
#if HAVE_NATIVE_mpn_mul_2
  { "mpn_mul_2",         speed_mpn_mul_2,     FLAG_R },
#endif

  { "mpn_divrem_1",      speed_mpn_divrem_1,  FLAG_R },
  { "mpn_divrem_1f",     speed_mpn_divrem_1f, FLAG_R },
#if HAVE_NATIVE_mpn_divrem_1c
  { "mpn_divrem_1c",     speed_mpn_divrem_1c, FLAG_R },
  { "mpn_divrem_1cf",    speed_mpn_divrem_1cf,FLAG_R },
#endif
  { "mpn_mod_1",         speed_mpn_mod_1,     FLAG_R },
#if HAVE_NATIVE_mpn_mod_1c
  { "mpn_mod_1c",        speed_mpn_mod_1c,    FLAG_R },
#endif
  { "mpn_preinv_divrem_1",  speed_mpn_preinv_divrem_1,  FLAG_R },
  { "mpn_preinv_divrem_1f", speed_mpn_preinv_divrem_1f, FLAG_R },
  { "mpn_preinv_mod_1",  speed_mpn_preinv_mod_1, FLAG_R },

  { "mpn_divrem_1_div",  speed_mpn_divrem_1_div,  FLAG_R },
  { "mpn_divrem_1_inv",  speed_mpn_divrem_1_inv,  FLAG_R },
  { "mpn_divrem_1f_div", speed_mpn_divrem_1f_div, FLAG_R },
  { "mpn_divrem_1f_inv", speed_mpn_divrem_1f_inv, FLAG_R },
  { "mpn_mod_1_div",     speed_mpn_mod_1_div,     FLAG_R },
  { "mpn_mod_1_inv",     speed_mpn_mod_1_inv,     FLAG_R },

  { "mpn_divrem_2",      speed_mpn_divrem_2,        },
  { "mpn_divrem_2_div",  speed_mpn_divrem_2_div,    },
  { "mpn_divrem_2_inv",  speed_mpn_divrem_2_inv,    },

  { "mpn_divexact_1",    speed_mpn_divexact_1,    FLAG_R },
  { "mpn_divexact_by3",  speed_mpn_divexact_by3          },

#if HAVE_NATIVE_mpn_modexact_1c_odd
  { "mpn_modexact_1_odd",  speed_mpn_modexact_1_odd,  FLAG_R },
#endif
  { "mpn_modexact_1c_odd", speed_mpn_modexact_1c_odd, FLAG_R },

  { "mpn_dc_tdiv_qr",       speed_mpn_dc_tdiv_qr       },
  { "mpn_dc_divrem_n",      speed_mpn_dc_divrem_n      },
  { "mpn_dc_divrem_sb",     speed_mpn_dc_divrem_sb     },
  { "mpn_dc_divrem_sb_div", speed_mpn_dc_divrem_sb_div },
  { "mpn_dc_divrem_sb_inv", speed_mpn_dc_divrem_sb_inv },

  { "mpn_sb_divrem_m3",     speed_mpn_sb_divrem_m3     },
  { "mpn_sb_divrem_m3_div", speed_mpn_sb_divrem_m3_div },
  { "mpn_sb_divrem_m3_inv", speed_mpn_sb_divrem_m3_inv },

  { "mpn_lshift",        speed_mpn_lshift, FLAG_R   },
  { "mpn_rshift",        speed_mpn_rshift, FLAG_R   },

  { "mpn_and_n",         speed_mpn_and_n,  FLAG_R_OPTIONAL },
  { "mpn_andn_n",        speed_mpn_andn_n, FLAG_R_OPTIONAL },
  { "mpn_nand_n",        speed_mpn_nand_n, FLAG_R_OPTIONAL },
  { "mpn_ior_n",         speed_mpn_ior_n,  FLAG_R_OPTIONAL },
  { "mpn_iorn_n",        speed_mpn_iorn_n, FLAG_R_OPTIONAL },
  { "mpn_nior_n",        speed_mpn_nior_n, FLAG_R_OPTIONAL },
  { "mpn_xor_n",         speed_mpn_xor_n,  FLAG_R_OPTIONAL },
  { "mpn_xnor_n",        speed_mpn_xnor_n, FLAG_R_OPTIONAL },
  { "mpn_com_n",         speed_mpn_com_n            },

  { "mpn_popcount",      speed_mpn_popcount         },
  { "mpn_hamdist",       speed_mpn_hamdist          },

  { "mpn_gcd_1",         speed_mpn_gcd_1,  FLAG_R_OPTIONAL },
  { "mpn_gcd_1N",        speed_mpn_gcd_1N, FLAG_R_OPTIONAL },

  { "mpn_gcd",           speed_mpn_gcd                    },
  { "mpn_gcd_binary",    speed_mpn_gcd_binary             },
  { "find_a",            speed_find_a,        FLAG_NODATA },
#if HAVE_NATIVE_mpn_gcd_finda
  { "mpn_gcd_finda",     speed_mpn_gcd_finda, FLAG_NODATA },
#endif

  { "mpn_gcdext",            speed_mpn_gcdext            },
  { "mpn_gcdext_single",     speed_mpn_gcdext_single     },
  { "mpn_gcdext_double",     speed_mpn_gcdext_double     },
  { "mpn_gcdext_one_single", speed_mpn_gcdext_one_single },
  { "mpn_gcdext_one_double", speed_mpn_gcdext_one_double },

  { "mpz_jacobi",        speed_mpz_jacobi           },
  { "mpn_jacobi_base",   speed_mpn_jacobi_base      },
  { "mpn_jacobi_base_1", speed_mpn_jacobi_base_1    },
  { "mpn_jacobi_base_2", speed_mpn_jacobi_base_2    },
  { "mpn_jacobi_base_3", speed_mpn_jacobi_base_3    },

  { "mpn_mul_basecase",  speed_mpn_mul_basecase, FLAG_R_OPTIONAL },
  { "mpn_sqr_basecase",  speed_mpn_sqr_basecase     },
#if HAVE_NATIVE_mpn_sqr_diagonal
  { "mpn_sqr_diagonal",  speed_mpn_sqr_diagonal     },
#endif

  { "mpn_mul_n",         speed_mpn_mul_n            },
  { "mpn_sqr_n",         speed_mpn_sqr_n            },

  { "mpn_kara_mul_n",    speed_mpn_kara_mul_n       },
  { "mpn_kara_sqr_n",    speed_mpn_kara_sqr_n       },
  { "mpn_toom3_mul_n",   speed_mpn_toom3_mul_n      },
  { "mpn_toom3_sqr_n",   speed_mpn_toom3_sqr_n      },
  { "mpn_mul_fft_full",      speed_mpn_mul_fft_full      },
  { "mpn_mul_fft_full_sqr",  speed_mpn_mul_fft_full_sqr  },

  { "mpn_mul_fft",       speed_mpn_mul_fft,     FLAG_R_OPTIONAL },
  { "mpn_mul_fft_sqr",   speed_mpn_mul_fft_sqr, FLAG_R_OPTIONAL },

  { "mpn_toom3_mul_n_mpn",   speed_mpn_toom3_mul_n_mpn   },
  { "mpn_toom3_mul_n_open",  speed_mpn_toom3_mul_n_open  },
  { "mpn_toom3_sqr_n_mpn",   speed_mpn_toom3_sqr_n_mpn   },
  { "mpn_toom3_sqr_n_open",  speed_mpn_toom3_sqr_n_open  },

  { "mpn_get_str",       speed_mpn_get_str,  FLAG_R_OPTIONAL },

  { "mpn_set_str",          speed_mpn_set_str,          FLAG_R_OPTIONAL },
  { "mpn_set_str_basecase", speed_mpn_set_str_basecase, FLAG_R_OPTIONAL },
  { "mpn_set_str_subquad",  speed_mpn_set_str_subquad,  FLAG_R_OPTIONAL },

  { "mpn_sqrtrem",       speed_mpn_sqrtrem          },

  { "mpn_fib2_ui",       speed_mpn_fib2_ui,    FLAG_NODATA },
  { "mpz_fib_ui",        speed_mpz_fib_ui,     FLAG_NODATA },
  { "mpz_fib2_ui",       speed_mpz_fib2_ui,    FLAG_NODATA },
  { "mpz_lucnum_ui",     speed_mpz_lucnum_ui,  FLAG_NODATA },
  { "mpz_lucnum2_ui",    speed_mpz_lucnum2_ui, FLAG_NODATA },

  { "mpz_add",           speed_mpz_add              },
  { "mpz_bin_uiui",      speed_mpz_bin_uiui, FLAG_NODATA | FLAG_R_OPTIONAL },
  { "mpz_fac_ui",        speed_mpz_fac_ui,   FLAG_NODATA   },
  { "mpz_powm",          speed_mpz_powm             },
  { "mpz_powm_mod",      speed_mpz_powm_mod         },
  { "mpz_powm_redc",     speed_mpz_powm_redc        },
  { "mpz_powm_ui",       speed_mpz_powm_ui          },

  { "mpz_mod",           speed_mpz_mod              },
  { "redc",              speed_redc                 },

  { "MPN_COPY",          speed_MPN_COPY             },
  { "MPN_COPY_INCR",     speed_MPN_COPY_INCR        },
  { "MPN_COPY_DECR",     speed_MPN_COPY_DECR        },
  { "memcpy",            speed_memcpy               },
#if HAVE_NATIVE_mpn_copyi
  { "mpn_copyi",         speed_mpn_copyi            },
#endif
#if HAVE_NATIVE_mpn_copyd
  { "mpn_copyd",         speed_mpn_copyd            },
#endif

  { "MPN_ZERO",          speed_MPN_ZERO             },

  { "modlimb_invert",       speed_modlimb_invert,       FLAG_NODATA },
  { "modlimb_invert_mul1",  speed_modlimb_invert_mul1,  FLAG_NODATA },
  { "modlimb_invert_loop",  speed_modlimb_invert_loop,  FLAG_NODATA },
  { "modlimb_invert_cond",  speed_modlimb_invert_cond,  FLAG_NODATA },
  { "modlimb_invert_arith", speed_modlimb_invert_arith, FLAG_NODATA },

  { "malloc_free",                  speed_malloc_free                  },
  { "malloc_realloc_free",          speed_malloc_realloc_free          },
  { "gmp_allocate_free",            speed_gmp_allocate_free            },
  { "gmp_allocate_reallocate_free", speed_gmp_allocate_reallocate_free },
  { "mpz_init_clear",               speed_mpz_init_clear               },
  { "mpq_init_clear",               speed_mpq_init_clear               },
  { "mpf_init_clear",               speed_mpf_init_clear               },
  { "mpz_init_realloc_clear",       speed_mpz_init_realloc_clear       },

  { "umul_ppmm",         speed_umul_ppmm,     FLAG_R_OPTIONAL },
#if HAVE_NATIVE_mpn_umul_ppmm
  { "mpn_umul_ppmm",     speed_mpn_umul_ppmm, FLAG_R_OPTIONAL },
#endif

  { "count_leading_zeros",  speed_count_leading_zeros,  FLAG_NODATA | FLAG_R_OPTIONAL },
  { "count_trailing_zeros", speed_count_trailing_zeros, FLAG_NODATA | FLAG_R_OPTIONAL },

  { "udiv_qrnnd",             speed_udiv_qrnnd,             FLAG_R_OPTIONAL },
  { "udiv_qrnnd_preinv",      speed_udiv_qrnnd_preinv,      FLAG_R_OPTIONAL },
  { "udiv_qrnnd_preinv2norm", speed_udiv_qrnnd_preinv2norm, FLAG_R_OPTIONAL },
  { "udiv_qrnnd_c",           speed_udiv_qrnnd_c,           FLAG_R_OPTIONAL },
#if HAVE_NATIVE_mpn_udiv_qrnnd
  { "mpn_udiv_qrnnd",         speed_mpn_udiv_qrnnd,         FLAG_R_OPTIONAL },
#endif
  { "invert_limb",            speed_invert_limb,            FLAG_R_OPTIONAL },

  { "operator_div",           speed_operator_div,           FLAG_R_OPTIONAL },
  { "operator_mod",           speed_operator_mod,           FLAG_R_OPTIONAL },

#ifdef SPEED_EXTRA_ROUTINES
  SPEED_EXTRA_ROUTINES
#endif
#ifdef SPEED_EXTRA_ROUTINES2
  SPEED_EXTRA_ROUTINES2
#endif
};


struct choice_t {
  const struct routine_t  *p;
  mp_limb_t               r;
  double                  scale;
  double                  time;
  int                     no_time;
  double                  prev_time;
  const char              *name;
};
struct choice_t  *choice;
int  num_choices = 0;


void
data_fill (mp_ptr ptr, mp_size_t size)
{
  switch (option_data) {
  case DATA_RANDOM:
    mpn_random (ptr, size);
    break;
  case DATA_RANDOM2:
    mpn_random2 (ptr, size);
    break;
  case DATA_ZEROS:
    MPN_ZERO (ptr, size);
    break;
  case DATA_AAS:
    MPN_FILL (ptr, size, GMP_NUMB_0xAA);
    break;
  case DATA_FFS:
    MPN_FILL (ptr, size, GMP_NUMB_MAX);
    break;
  case DATA_2FD:
    MPN_FILL (ptr, size, GMP_NUMB_MAX);
    ptr[0] -= 2;
    break;
  default:
    abort();
    /*NOTREACHED*/
  }
}

/* The code here handling the various combinations of output options isn't
   too attractive, but it works and is fairly clean.  */

#define SIZE_TO_DIVISOR(n)              \
  (option_square == 1 ? (n)*(n)         \
  : option_square == 2 ? (n)*((n)+1)/2  \
  : (n))

void
run_one (FILE *fp, struct speed_params *s, mp_size_t prev_size)
{
  const char  *first_open_fastest, *first_open_notfastest, *first_close;
  int         i, fastest, want_data;
  double      fastest_time;
  TMP_DECL (marker);

  TMP_MARK (marker);

  /* allocate data, unless all routines are NODATA */
  want_data = 0;
  for (i = 0; i < num_choices; i++)
    want_data |= ((choice[i].p->flag & FLAG_NODATA) == 0);

  if (want_data)
    {
      sp.xp = SPEED_TMP_ALLOC_LIMBS (s->size, s->align_xp);
      sp.yp = SPEED_TMP_ALLOC_LIMBS (s->size, s->align_yp);

      data_fill (s->xp, s->size);
      data_fill (s->yp, s->size);
    }
  else
    {
      sp.xp = NULL;
      sp.yp = NULL;
    }

  if (prev_size == -1 && option_cmp == CMP_DIFFPREV)
    {
      first_open_fastest = "(#";
      first_open_notfastest = " (";
      first_close = ")";
    }
  else
    {
      first_open_fastest = "#";
      first_open_notfastest = " ";
      first_close = "";
    }

  fastest = -1;
  fastest_time = -1.0;
  for (i = 0; i < num_choices; i++)
    {
      s->r = choice[i].r;
      choice[i].time = speed_measure (choice[i].p->fun, s);
      choice[i].no_time = (choice[i].time == -1.0);
      choice[i].time *= choice[i].scale;

      /* Apply the effect of CMP_DIFFPREV, but the new choice[i].prev_time
         is before any differences.  */
      {
        double     t;
        t = choice[i].time;
        if (t != -1.0 && option_cmp == CMP_DIFFPREV && prev_size != -1)
          {
            if (choice[i].prev_time == -1.0)
              choice[i].no_time = 1;
            else
              choice[i].time = choice[i].time - choice[i].prev_time;
          }
        choice[i].prev_time = t;
      }

      if (choice[i].no_time)
        continue;

      /* Look for the fastest after CMP_DIFFPREV has been applied, but
         before CMP_RATIO or CMP_DIFFERENCE.  There's only a fastest shown
         if there's more than one routine.  */
      if (num_choices > 1 && (fastest == -1 || choice[i].time < fastest_time))
        {
          fastest = i;
          fastest_time = choice[i].time;
        }

      if (option_cmp == CMP_DIFFPREV)
        {
          /* Conversion for UNIT_CYCLESPERLIMB differs in CMP_DIFFPREV. */
          if (option_unit == UNIT_CYCLES)
            choice[i].time /= speed_cycletime;
          else if (option_unit == UNIT_CYCLESPERLIMB)
            {
              if (prev_size == -1)
                choice[i].time /= speed_cycletime;
              else
                choice[i].time /=  (speed_cycletime
                                    * (SIZE_TO_DIVISOR(s->size)
                                       - SIZE_TO_DIVISOR(prev_size)));
            }
        }
      else
        {
          if (option_unit == UNIT_CYCLES)
            choice[i].time /= speed_cycletime;
          else if (option_unit == UNIT_CYCLESPERLIMB)
            choice[i].time /= (speed_cycletime * SIZE_TO_DIVISOR(s->size));
          
          if (option_cmp == CMP_RATIO && i > 0)
            {
              /* A ratio isn't affected by the units chosen. */
              if (choice[0].no_time || choice[0].time == 0.0)
                choice[i].no_time = 1;
              else
                choice[i].time /= choice[0].time;
            }
          else if (option_cmp == CMP_DIFFERENCE && i > 0)
            {
              if (choice[0].no_time)
                {
                  choice[i].no_time = 1;
                  continue;
                }
              choice[i].time -= choice[0].time;
            }
        }
    }

  if (option_gnuplot)
    {
      /* In CMP_DIFFPREV, don't print anything for the first size, start
         with the second where an actual difference is available.

         In CMP_RATIO, print the first column as 1.0.

         The 9 decimals printed is much more than the expected precision of
         the measurements actually. */

      if (! (option_cmp == CMP_DIFFPREV && prev_size == -1))
        {     
          fprintf (fp, "%-6ld ", s->size);
          for (i = 0; i < num_choices; i++)
            fprintf (fp, "  %.9e", 
                     choice[i].no_time ? 0.0
                     : (option_cmp == CMP_RATIO && i == 0) ? 1.0
                     : choice[i].time);
          fprintf (fp, "\n");
        }
    }
  else
    {
      fprintf (fp, "%-6ld ", s->size);
      for (i = 0; i < num_choices; i++)
        {
          char  buf[128];
          int   decimals;

          if (choice[i].no_time)
            decimals = 0, choice[i].time = 0.0;
          else if (option_unit == UNIT_CYCLESPERLIMB
                   || (option_cmp == CMP_RATIO && i > 0))
            decimals = 4;
          else if (option_unit == UNIT_CYCLES)
            decimals = 2;
          else
            decimals = 9;

          sprintf (buf, "%s%.*f%s",
                   i == fastest ? first_open_fastest : first_open_notfastest,
                   decimals, choice[i].time, first_close);
          fprintf (fp, " %*s", COLUMN_WIDTH, buf);
        }
      fprintf (fp, "\n");
    }

  TMP_FREE (marker);
}

void
run_all (FILE *fp)
{
  mp_size_t  prev_size;
  int        i;
  TMP_DECL (marker);

  TMP_MARK (marker);
  sp.xp_block = SPEED_TMP_ALLOC_LIMBS (SPEED_BLOCK_SIZE, sp.align_xp);
  sp.yp_block = SPEED_TMP_ALLOC_LIMBS (SPEED_BLOCK_SIZE, sp.align_yp);

  data_fill (sp.xp_block, SPEED_BLOCK_SIZE);
  data_fill (sp.yp_block, SPEED_BLOCK_SIZE);

  for (i = 0; i < size_num; i++)
    {
      sp.size = size_array[i].start;
      prev_size = -1;
      for (;;)
        {
          mp_size_t  step;

          if (option_data == DATA_2FD && sp.size >= 2)
            sp.xp[sp.size-1] = 2;

          run_one (fp, &sp, prev_size);
          prev_size = sp.size;

          if (option_data == DATA_2FD && sp.size >= 2)
            sp.xp[sp.size-1] = MP_LIMB_T_MAX;

          if (option_factor != 0.0)
            {
              step = (mp_size_t) (sp.size * option_factor - sp.size);
              if (step < 1)
                step = 1;
            }
          else
            step = 1;
          if (step < option_step)
            step = option_step;

          sp.size += step;
          if (sp.size > size_array[i].end)
            break;
        }
    }

  TMP_FREE (marker);
}


FILE *
fopen_for_write (const char *filename)
{
  FILE  *fp;
  if ((fp = fopen (filename, "w")) == NULL)
    {
      fprintf (stderr, "Cannot create %s\n", filename);
      exit(1);
    }
  return fp;
}
      
void
fclose_written (FILE *fp, const char *filename)
{
  int  err;

  err = ferror (fp);
  err |= fclose (fp);

  if (err)
    {
      fprintf (stderr, "Error writing %s\n", filename);
      exit(1);
    }
}


void
run_gnuplot (int argc, char *argv[])
{
  char  *plot_filename;
  char  *data_filename;
  FILE  *fp;
  int   i;
     
  plot_filename = (char *) (*__gmp_allocate_func)
    (strlen (option_gnuplot_basename) + 20);
  data_filename = (char *) (*__gmp_allocate_func)
    (strlen (option_gnuplot_basename) + 20);
      
  sprintf (plot_filename, "%s.gnuplot", option_gnuplot_basename);
  sprintf (data_filename, "%s.data",    option_gnuplot_basename);

  fp = fopen_for_write (plot_filename);

  fprintf (fp, "# Generated with:\n");
  fprintf (fp, "#");
  for (i = 0; i < argc; i++)
    fprintf (fp, " %s", argv[i]);
  fprintf (fp, "\n");
  fprintf (fp, "\n");

  /* Putting the key at the top left is usually good, and you can change it
     interactively if it's not. */
  fprintf (fp, "set key left\n");

  /* designed to make it possible to see crossovers easily */
  fprintf (fp, "set data style linespoints\n");

  fprintf (fp, "plot ");
  for (i = 0; i < num_choices; i++)
    {
      fprintf (fp, " \"%s\" using 1:%d", data_filename, i+2);
      fprintf (fp, " title \"%s\"", choice[i].name);

      if (i != num_choices-1)
        fprintf (fp, ", \\");
      fprintf (fp, "\n");
    }

  fprintf (fp, "load \"-\"\n");
  fclose_written (fp, plot_filename);

  fp = fopen_for_write (data_filename);

  /* Unbuffered so you can see where the program was up to if it crashes or
     you kill it. */
  setbuf (fp, NULL);

  run_all (fp);
  fclose_written (fp, data_filename);
}


/* Return a limb with n many one bits (starting from the least significant) */

#define LIMB_ONES(n) \
  ((n) == BITS_PER_MP_LIMB ? MP_LIMB_T_MAX      \
    : (n) == 0 ? CNST_LIMB(0)                   \
    : (CNST_LIMB(1) << (n)) - 1)

mp_limb_t
r_string (const char *s)
{
  const char  *s_orig = s;
  long        n;

  if (strcmp (s, "aas") == 0)
    return GMP_NUMB_0xAA;

  {
    mpz_t      z;
    mp_limb_t  l;
    int        set, siz;

    mpz_init (z);
    set = mpz_set_str (z, s, 0);
    siz = SIZ(z);
    l = (siz == 0 ? 0 : siz > 0 ? PTR(z)[0] : -PTR(z)[0]);
    mpz_clear (z);
    if (set == 0)
      {
        if (siz > 1 || siz < -1)
          printf ("Warning, r parameter %s truncated to %d bits\n",
                  s_orig, BITS_PER_MP_LIMB);
        return l;
      }
  }

  if (s[0] == '0' && (s[1] == 'x' || s[1] == 'X'))
    n = strtoul (s+2, (char **) &s, 16);
  else
    n = strtol (s, (char **) &s, 10);

  if (strcmp (s, "bits") == 0)
    {
      mp_limb_t  l;
      if (n > BITS_PER_MP_LIMB)
        {
          fprintf (stderr, "%ld bit parameter invalid (max %d bits)\n", 
                   n, BITS_PER_MP_LIMB);
          exit (1);
        }
      mpn_random (&l, 1);
      return (l | (1 << (n-1))) & LIMB_ONES(n);
    }
  else  if (strcmp (s, "ones") == 0)
    {
      if (n > BITS_PER_MP_LIMB)
        {
          fprintf (stderr, "%ld bit parameter invalid (max %d bits)\n", 
                   n, BITS_PER_MP_LIMB);
          exit (1);
        }
      return LIMB_ONES (n);
    }
  else if (*s != '\0')
    {
      fprintf (stderr, "invalid r parameter: %s\n", s_orig);
      exit (1);
    }

  return n;
}


void
routine_find (struct choice_t *c, const char *s_orig)
{
  const char  *s;
  int     i;
  size_t  nlen;

  c->name = s_orig;
  s = strchr (s_orig, '*');
  if (s != NULL)
    {
      c->scale = atof(s_orig);
      s++;
    }
  else
    {
      c->scale = 1.0;
      s = s_orig;
    }

  for (i = 0; i < numberof (routine); i++)
    {
      nlen = strlen (routine[i].name);
      if (memcmp (s, routine[i].name, nlen) != 0)
        continue;

      if (s[nlen] == '.')
        {
          /* match, with a .r parameter */

          if (! (routine[i].flag & (FLAG_R|FLAG_R_OPTIONAL)))
            {
              fprintf (stderr,
                       "Choice %s bad: doesn't take a \".<r>\" parameter\n",
                       s_orig);
              exit (1);
            }

          c->p = &routine[i];
          c->r = r_string (s + nlen + 1);
          return;
        }

      if (s[nlen] == '\0')
        {
          /* match, with no parameter */

          if (routine[i].flag & FLAG_R)
            {
              fprintf (stderr,
                       "Choice %s bad: needs a \".<r>\" parameter\n",
                       s_orig);
              exit (1);
            }

          c->p = &routine[i];
          c->r = 0;
          return;
        }
    }

  fprintf (stderr, "Choice %s unrecognised\n", s_orig);
  exit (1);
}


void
usage (void)
{
  int  i;
  
  speed_time_init ();

  printf ("\
Usage: speed [-options] -s size <routine>...\n\
Measure the speed of some routines.\n\
Times are in seconds, accuracy is shown.\n\
\n\
   -p num     set precision as number of time units each routine must run\n\
   -s size[-end][,size[-end]]...   sizes to measure\n\
              single sizes or ranges, sep with comma or use multiple -s\n\
   -t step    step through sizes by given amount\n\
   -f factor  step through sizes by given factor (eg. 1.05)\n\
   -r         show times as ratios of the first routine\n\
   -d         show times as difference from the first routine\n\
   -D         show times as difference from previous size shown\n\
   -c         show times in CPU cycles\n\
   -C         show times in cycles per limb\n\
   -u         print resource usage (memory) at end\n\
   -P name    output plot files \"name.gnuplot\" and \"name.data\"\n\
   -a <type>  use given data: random(default), random2, zeros, aas, ffs, 2fd\n\
   -x, -y, -w, -W <align>  specify data alignments, sources and dests\n\
   -o addrs   print addresses of data blocks\n\
\n\
If both -t and -f are used, it means step by the factor or the step, whichever\n\
is greater.\n\
If both -C and -D are used, it means cycles per however many limbs between a\n\
size and the previous size.\n\
\n\
After running with -P, plots can be viewed with Gnuplot or Quickplot.\n\
\"gnuplot name.gnuplot\" (use \"set logscale xy; replot\" at the prompt for\n\
a log/log plot).\n\
\"quickplot -s name.data\" (has interactive zooming, and note -s is important\n\
when viewing more than one routine, it means same axis scales for all data).\n\
\n\
The available routines are as follows.\n\
\n\
");

  for (i = 0; i < numberof (routine); i++)
    {
      if (routine[i].flag & FLAG_R) 
        printf ("\t%s.r\n", routine[i].name); 
      else if (routine[i].flag & FLAG_R_OPTIONAL) 
        printf ("\t%s (optional .r)\n", routine[i].name); 
      else
        printf ("\t%s\n", routine[i].name); 
    }
      
  printf ("\n\
Routines with a \".r\" need an extra parameter, for example mpn_lshift.6\n\
r should be in decimal, or use 0xN for hexadecimal.\n\
\n\
Special forms for r are \"<N>bits\" for a random N bit number, \"<N>ones\" for\n\
N one bits, or \"aas\" for 0xAA..AA.\n\
\n\
Times for sizes out of the range accepted by a routine are shown as 0.\n\
The fastest routine at each size is marked with a # (free form output only).\n\
\n\
%s\
\n\
Gnuplot home page http://www.cs.dartmouth.edu/gnuplot_info.html\n\
Quickplot home page http://www.kachinatech.com/~quickplot\n\
", speed_time_string);
}

int
main (int argc, char *argv[])
{
  int  i;
  int  opt;

  /* Unbuffered so output goes straight out when directed to a pipe or file
     and isn't lost on killing the program half way.  */
  setbuf (stdout, NULL);

  for (;;)
    {
      opt = getopt(argc, argv, "a:CcDdEFf:o:p:P:rRs:t:ux:y:w:W:z");
      if (opt == EOF)
        break;

      switch (opt) {
      case 'a':
        if (strcmp (optarg, "random") == 0)       option_data = DATA_RANDOM;
        else if (strcmp (optarg, "random2") == 0) option_data = DATA_RANDOM2;
        else if (strcmp (optarg, "zeros") == 0)   option_data = DATA_ZEROS;
        else if (strcmp (optarg, "aas") == 0)     option_data = DATA_AAS;
        else if (strcmp (optarg, "ffs") == 0)     option_data = DATA_FFS;
        else if (strcmp (optarg, "2fd") == 0)     option_data = DATA_2FD;
        else
          {
            fprintf (stderr, "unrecognised data option: %s\n", optarg);
            exit (1);
          }
        break;
      case 'C':
        if (option_unit  != UNIT_SECONDS) goto bad_unit;
        option_unit = UNIT_CYCLESPERLIMB;
        break;
      case 'c':
        if (option_unit != UNIT_SECONDS)
          {
          bad_unit:
            fprintf (stderr, "cannot use more than one of -c, -C\n");
            exit (1);
          }
        option_unit = UNIT_CYCLES;
        break;
      case 'D':
        if (option_cmp != CMP_ABSOLUTE) goto bad_cmp;
        option_cmp = CMP_DIFFPREV;
        break;
      case 'd':
        if (option_cmp != CMP_ABSOLUTE)
          {
          bad_cmp:
            fprintf (stderr, "cannot use more than one of -d, -D, -r\n");
            exit (1);
          }
        option_cmp = CMP_DIFFERENCE;
        break;
      case 'E':
        option_square = 1;
        break;
      case 'F':
        option_square = 2;
        break;
      case 'f':
        option_factor = atof (optarg);
        if (option_factor <= 1.0)
          {
            fprintf (stderr, "-f factor must be > 1.0\n");
            exit (1);
          }
        break;
      case 'o':
        speed_option_set (optarg);
        break;
      case 'P':
        option_gnuplot = 1;
        option_gnuplot_basename = optarg;
        break;
      case 'p':
        speed_precision = atoi (optarg);
        break;
      case 'R':
        option_seed = time (NULL);
        break;
      case 'r':
        if (option_cmp != CMP_ABSOLUTE)
          goto bad_cmp;
        option_cmp = CMP_RATIO;
        break;
      case 's':
        {
          char  *s;
          for (s = strtok (optarg, ","); s != NULL; s = strtok (NULL, ","))
            {
              if (size_num == size_allocnum)
                {
                  size_array = (struct size_array_t *)
                    __gmp_allocate_or_reallocate
                    (size_array,
                     size_allocnum * sizeof(size_array[0]),
                     (size_allocnum+10) * sizeof(size_array[0]));
                  size_allocnum += 10;
                }
              if (sscanf (s, "%ld-%ld",
                          &size_array[size_num].start,
                          &size_array[size_num].end) != 2)
                {
                  size_array[size_num].start = size_array[size_num].end
                    = atol (s);
                }

              if (size_array[size_num].start < 0
                  || size_array[size_num].end < 0
                  || size_array[size_num].start > size_array[size_num].end)
                {
                  fprintf (stderr, "invalid size parameter: %s\n", s);
                  exit (1);
                }

              size_num++;
            }
        }
        break;
      case 't':
        option_step = atol (optarg);
        if (option_step < 1)
          {
            fprintf (stderr, "-t step must be >= 1\n");
            exit (1);
          }
        break;
      case 'u':
        option_resource_usage = 1;
        break;
      case 'z':
        sp.cache = 1;
        break;
      case 'x':
        sp.align_xp = atol (optarg);
        break;
      case 'y':
        sp.align_yp = atol (optarg);
        break;
      case 'w':
        sp.align_wp = atol (optarg);
        break;
      case 'W':
        sp.align_wp2 = atol (optarg);
        break;
      case '?':
        exit(1);
      }
    }

  if (optind >= argc)
    {
      usage ();
      exit (1);
    }

  if (size_num == 0)
    {
      fprintf (stderr, "-s <size> must be specified\n");
      exit (1);
    }

  gmp_randseed_ui (RANDS, option_seed);

  choice = (struct choice_t *) (*__gmp_allocate_func)
    ((argc - optind) * sizeof(choice[0]));
  for ( ; optind < argc; optind++)
    {
      struct choice_t  c;
      routine_find (&c, argv[optind]);
      choice[num_choices] = c;
      num_choices++;
    }
  
  if ((option_cmp == CMP_RATIO || option_cmp == CMP_DIFFERENCE) &&
      num_choices < 2)
    {
      fprintf (stderr, "WARNING, -d or -r does nothing when only one routine requested\n");
    }

  speed_time_init ();
  if (option_unit == UNIT_CYCLES || option_unit == UNIT_CYCLESPERLIMB)
    speed_cycletime_need_cycles ();
  else
    speed_cycletime_need_seconds ();

  if (option_gnuplot)
    {
      run_gnuplot (argc, argv);
    }
  else
    {
      if (option_unit == UNIT_SECONDS)
        printf ("overhead %.9f secs", speed_measure (speed_noop, NULL));
      else
        printf ("overhead %.2f cycles",
                speed_measure (speed_noop, NULL) / speed_cycletime);
      printf (", precision %d units of %.2e secs",
              speed_precision, speed_unittime);
      
      if (speed_cycletime == 1.0 || speed_cycletime == 0.0)
        printf (", CPU freq unknown\n");
      else
        printf (", CPU freq %.2f MHz\n", 1e-6/speed_cycletime);

      printf ("       ");
      for (i = 0; i < num_choices; i++)
        printf (" %*s", COLUMN_WIDTH, choice[i].name); 
      printf ("\n");

      run_all (stdout);
    }

  if (option_resource_usage)
    {
#if HAVE_GETRUSAGE
      {
        /* This doesn't give data sizes on linux 2.0.x, only utime. */
        struct rusage  r;
        if (getrusage (RUSAGE_SELF, &r) != 0)
          perror ("getrusage");
        else
          printf ("getrusage(): utime %ld.%06ld data %ld stack %ld maxresident %ld\n",
                  r.ru_utime.tv_sec, r.ru_utime.tv_usec,
                  r.ru_idrss, r.ru_isrss, r.ru_ixrss); 
      }
#else
      printf ("getrusage() not available\n");
#endif

      /* Linux kernel. */
      {
        char  buf[128];
        sprintf (buf, "/proc/%d/status", getpid());
        if (access (buf, R_OK) == 0)
          {
            sprintf (buf, "cat /proc/%d/status", getpid());
            system (buf);
          }

      }
    }

  return 0;
}


