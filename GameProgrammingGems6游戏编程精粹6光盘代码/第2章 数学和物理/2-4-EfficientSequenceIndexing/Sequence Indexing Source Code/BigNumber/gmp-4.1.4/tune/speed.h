/* Header for speed and threshold things.

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

#ifndef __SPEED_H__
#define __SPEED_H__


/* Pad ptr,oldsize with zero limbs (at the most significant end) to make it
   newsize long. */
#define MPN_ZERO_EXTEND(ptr, oldsize, newsize)          \
  do {                                                  \
    ASSERT ((newsize) >= (oldsize));                    \
    MPN_ZERO ((ptr)+(oldsize), (newsize)-(oldsize));    \
  } while (0)

/* A mask of the least significant n bits.  Note 1<<32 doesn't give zero on
   x86 family CPUs, hence the separate case for BITS_PER_MP_LIMB. */
#define MP_LIMB_T_LOWBITMASK(n) \
  ((n) == BITS_PER_MP_LIMB ? MP_LIMB_T_MAX : ((mp_limb_t) 1 << (n)) - 1)


/* align must be a power of 2 here, usually CACHE_LINE_SIZE is a good choice */

#define TMP_ALLOC_ALIGNED(bytes, align) \
  align_pointer (TMP_ALLOC ((bytes) + (align)-1), (align))
#define TMP_ALLOC_LIMBS_ALIGNED(limbs, align) \
  ((mp_ptr) TMP_ALLOC_ALIGNED ((limbs)*sizeof(mp_limb_t), align))

/* 32 for pentium, 64 for athlon, might want to configure this for other
   CPUs.  In truth though nothing has yet shown up that cares about cache
   line boundaries.  The only practical effect of this is to restrict the
   range that s->align_xp can take.  Perhaps this could be a variable
   instead. */
#define CACHE_LINE_SIZE    64 /* bytes */

#define SPEED_TMP_ALLOC_ADJUST_MASK  (CACHE_LINE_SIZE/BYTES_PER_MP_LIMB - 1)

#define SPEED_TMP_ALLOC_LIMBS(limbs, align) \
  (speed_tmp_alloc_adjust             \
    (TMP_ALLOC_LIMBS((limbs) + SPEED_TMP_ALLOC_ADJUST_MASK), (align)))


/* This is the size for s->xp_block and s->yp_block, used in certain
   routines that want to run across many different data values and use
   s->size for a different purpose, eg. SPEED_ROUTINE_MPN_GCD_1.

   512 means 2kbytes of data for each of xp_block and yp_block, making 4k
   total, which should fit easily in any L1 data cache. */

#define SPEED_BLOCK_SIZE   512 /* limbs */


extern double  speed_unittime;
extern double  speed_cycletime;
extern int     speed_precision;
extern char    speed_time_string[];
void speed_time_init _PROTO ((void));
void speed_cycletime_fail _PROTO ((const char *str));
void speed_cycletime_init _PROTO ((void));
void speed_cycletime_need_cycles _PROTO ((void));
void speed_cycletime_need_seconds _PROTO ((void));
void speed_starttime _PROTO ((void));
double speed_endtime _PROTO ((void));


struct speed_params {
  unsigned   reps;      /* how many times to run the routine */
  mp_ptr     xp;        /* first argument */
  mp_ptr     yp;        /* second argument */
  mp_size_t  size;      /* size of both arguments */
  mp_limb_t  r;         /* user supplied parameter */
  mp_size_t  align_xp;  /* alignment of xp */
  mp_size_t  align_yp;  /* alignment of yp */
  mp_size_t  align_wp;  /* intended alignment of wp */
  mp_size_t  align_wp2; /* intended alignment of wp2 */
  mp_ptr     xp_block;  /* first special SPEED_BLOCK_SIZE block */
  mp_ptr     yp_block;  /* second special SPEED_BLOCK_SIZE block */

  double     time_divisor; /* optionally set by the speed routine */
  
  /* used by the cache priming things */
  int        cache;
  unsigned   src_num, dst_num;
  struct {
    mp_ptr    ptr;
    mp_size_t size;
  } src[2], dst[3];
};

typedef double (*speed_function_t) _PROTO ((struct speed_params *s));

double speed_measure _PROTO ((speed_function_t fun, struct speed_params *s));

/* Prototypes for speed measuring routines */

double speed_back_to_back (struct speed_params *s);
double speed_count_leading_zeros _PROTO ((struct speed_params *s));
double speed_count_trailing_zeros _PROTO ((struct speed_params *s));
double speed_find_a _PROTO ((struct speed_params *s));
double speed_gmp_allocate_free _PROTO ((struct speed_params *s));
double speed_gmp_allocate_reallocate_free _PROTO ((struct speed_params *s));
double speed_invert_limb _PROTO ((struct speed_params *s));
double speed_malloc_free _PROTO ((struct speed_params *s));
double speed_malloc_realloc_free _PROTO ((struct speed_params *s));
double speed_memcpy _PROTO ((struct speed_params *s));
double speed_modlimb_invert _PROTO ((struct speed_params *s));
double speed_modlimb_invert_mul1 _PROTO ((struct speed_params *s));
double speed_modlimb_invert_loop _PROTO ((struct speed_params *s));
double speed_modlimb_invert_cond _PROTO ((struct speed_params *s));
double speed_modlimb_invert_arith _PROTO ((struct speed_params *s));

double speed_mpf_init_clear _PROTO ((struct speed_params *s));

double speed_mpn_add_n _PROTO ((struct speed_params *s));
double speed_mpn_and_n _PROTO ((struct speed_params *s));
double speed_mpn_andn_n _PROTO ((struct speed_params *s));
double speed_mpn_addmul_1 _PROTO ((struct speed_params *s));
double speed_mpn_com_n _PROTO ((struct speed_params *s));
double speed_mpn_copyd _PROTO ((struct speed_params *s));
double speed_mpn_copyi _PROTO ((struct speed_params *s));
double speed_mpn_dc_divrem_n _PROTO ((struct speed_params *s));
double speed_mpn_dc_divrem_sb _PROTO ((struct speed_params *s));
double speed_mpn_dc_divrem_sb_div _PROTO ((struct speed_params *s));
double speed_mpn_dc_divrem_sb_inv _PROTO ((struct speed_params *s));
double speed_mpn_dc_tdiv_qr _PROTO ((struct speed_params *s));
double speed_MPN_COPY _PROTO ((struct speed_params *s));
double speed_MPN_COPY_DECR _PROTO ((struct speed_params *s));
double speed_MPN_COPY_INCR _PROTO ((struct speed_params *s));
double speed_mpn_divexact_1 _PROTO ((struct speed_params *s));
double speed_mpn_divexact_by3 _PROTO ((struct speed_params *s));
double speed_mpn_divrem_1 _PROTO ((struct speed_params *s));
double speed_mpn_divrem_1f _PROTO ((struct speed_params *s));
double speed_mpn_divrem_1c _PROTO ((struct speed_params *s));
double speed_mpn_divrem_1cf _PROTO ((struct speed_params *s));
double speed_mpn_divrem_1_div _PROTO ((struct speed_params *s));
double speed_mpn_divrem_1f_div _PROTO ((struct speed_params *s));
double speed_mpn_divrem_1_inv _PROTO ((struct speed_params *s));
double speed_mpn_divrem_1f_inv _PROTO ((struct speed_params *s));
double speed_mpn_divrem_2 _PROTO ((struct speed_params *s));
double speed_mpn_divrem_2_div _PROTO ((struct speed_params *s));
double speed_mpn_divrem_2_inv _PROTO ((struct speed_params *s));
double speed_mpn_fib2_ui _PROTO ((struct speed_params *s));
double speed_mpn_gcd _PROTO ((struct speed_params *s));
double speed_mpn_gcd_finda _PROTO ((struct speed_params *s));
double speed_mpn_gcd_1 _PROTO ((struct speed_params *s));
double speed_mpn_gcd_1N _PROTO ((struct speed_params *s));
double speed_mpn_gcd_binary _PROTO ((struct speed_params *s));
double speed_mpn_gcd_finda _PROTO ((struct speed_params *s));
double speed_mpn_gcdext _PROTO ((struct speed_params *s));
double speed_mpn_gcdext_double _PROTO ((struct speed_params *s));
double speed_mpn_gcdext_one_double _PROTO ((struct speed_params *s));
double speed_mpn_gcdext_one_single _PROTO ((struct speed_params *s));
double speed_mpn_gcdext_single _PROTO ((struct speed_params *s));
double speed_mpn_get_str _PROTO ((struct speed_params *s));
double speed_mpn_hamdist _PROTO ((struct speed_params *s));
double speed_mpn_ior_n _PROTO ((struct speed_params *s));
double speed_mpn_iorn_n _PROTO ((struct speed_params *s));
double speed_mpn_jacobi_base _PROTO ((struct speed_params *s));
double speed_mpn_jacobi_base_1 _PROTO ((struct speed_params *s));
double speed_mpn_jacobi_base_2 _PROTO ((struct speed_params *s));
double speed_mpn_jacobi_base_3 _PROTO ((struct speed_params *s));
double speed_mpn_kara_mul_n _PROTO ((struct speed_params *s));
double speed_mpn_kara_sqr_n _PROTO ((struct speed_params *s));
double speed_mpn_lshift _PROTO ((struct speed_params *s));
double speed_mpn_mod_1 _PROTO ((struct speed_params *s));
double speed_mpn_mod_1c _PROTO ((struct speed_params *s));
double speed_mpn_mod_1_div _PROTO ((struct speed_params *s));
double speed_mpn_mod_1_inv _PROTO ((struct speed_params *s));
double speed_mpn_mod_34lsub1 _PROTO ((struct speed_params *s));
double speed_mpn_modexact_1_odd _PROTO ((struct speed_params *s));
double speed_mpn_modexact_1c_odd _PROTO ((struct speed_params *s));
double speed_mpn_mul_1 _PROTO ((struct speed_params *s));
double speed_mpn_mul_1_inplace _PROTO ((struct speed_params *s));
double speed_mpn_mul_2 _PROTO ((struct speed_params *s));
double speed_mpn_mul_basecase _PROTO ((struct speed_params *s));
double speed_mpn_mul_fft _PROTO ((struct speed_params *s));
double speed_mpn_mul_fft_sqr _PROTO ((struct speed_params *s));
double speed_mpn_mul_fft_full _PROTO ((struct speed_params *s));
double speed_mpn_mul_fft_full_sqr _PROTO ((struct speed_params *s));
double speed_mpn_mul_n _PROTO ((struct speed_params *s));
double speed_mpn_mul_n_sqr _PROTO ((struct speed_params *s));
double speed_mpn_nand_n _PROTO ((struct speed_params *s));
double speed_mpn_nior_n _PROTO ((struct speed_params *s));
double speed_mpn_popcount _PROTO ((struct speed_params *s));
double speed_mpn_preinv_divrem_1 _PROTO ((struct speed_params *s));
double speed_mpn_preinv_divrem_1f _PROTO ((struct speed_params *s));
double speed_mpn_preinv_mod_1 _PROTO ((struct speed_params *s));
double speed_redc _PROTO ((struct speed_params *s));
double speed_mpn_rshift _PROTO ((struct speed_params *s));
double speed_mpn_sb_divrem_m3 _PROTO ((struct speed_params *s));
double speed_mpn_sb_divrem_m3_div _PROTO ((struct speed_params *s));
double speed_mpn_sb_divrem_m3_inv _PROTO ((struct speed_params *s));
double speed_mpn_set_str _PROTO ((struct speed_params *s));
double speed_mpn_set_str_basecase _PROTO ((struct speed_params *s));
double speed_mpn_set_str_subquad _PROTO ((struct speed_params *s));
double speed_mpn_sqr_basecase _PROTO ((struct speed_params *s));
double speed_mpn_sqr_diagonal _PROTO ((struct speed_params *s));
double speed_mpn_sqr_n _PROTO ((struct speed_params *s));
double speed_mpn_sqrtrem _PROTO ((struct speed_params *s));
double speed_mpn_sub_n _PROTO ((struct speed_params *s));
double speed_mpn_submul_1 _PROTO ((struct speed_params *s));
double speed_mpn_toom3_mul_n _PROTO ((struct speed_params *s));
double speed_mpn_toom3_mul_n_mpn _PROTO ((struct speed_params *s));
double speed_mpn_toom3_mul_n_open _PROTO ((struct speed_params *s));
double speed_mpn_toom3_sqr_n _PROTO ((struct speed_params *s));
double speed_mpn_toom3_sqr_n_mpn _PROTO ((struct speed_params *s));
double speed_mpn_toom3_sqr_n_open _PROTO ((struct speed_params *s));
double speed_mpn_udiv_qrnnd _PROTO ((struct speed_params *s));
double speed_mpn_umul_ppmm _PROTO ((struct speed_params *s));
double speed_mpn_xnor_n _PROTO ((struct speed_params *s));
double speed_mpn_xor_n _PROTO ((struct speed_params *s));
double speed_MPN_ZERO _PROTO ((struct speed_params *s));

double speed_mpq_init_clear _PROTO ((struct speed_params *s));

double speed_mpz_add _PROTO ((struct speed_params *s));
double speed_mpz_bin_uiui _PROTO ((struct speed_params *s));
double speed_mpz_fac_ui _PROTO ((struct speed_params *s));
double speed_mpz_fib_ui _PROTO ((struct speed_params *s));
double speed_mpz_fib2_ui _PROTO ((struct speed_params *s));
double speed_mpz_init_clear _PROTO ((struct speed_params *s));
double speed_mpz_init_realloc_clear _PROTO ((struct speed_params *s));
double speed_mpz_jacobi _PROTO ((struct speed_params *s));
double speed_mpz_lucnum_ui _PROTO ((struct speed_params *s));
double speed_mpz_lucnum2_ui _PROTO ((struct speed_params *s));
double speed_mpz_mod _PROTO ((struct speed_params *s));
double speed_mpz_powm _PROTO ((struct speed_params *s));
double speed_mpz_powm_mod _PROTO ((struct speed_params *s));
double speed_mpz_powm_redc _PROTO ((struct speed_params *s));
double speed_mpz_powm_ui _PROTO ((struct speed_params *s));

double speed_noop _PROTO ((struct speed_params *s));
double speed_noop_wxs _PROTO ((struct speed_params *s));
double speed_noop_wxys _PROTO ((struct speed_params *s));

double speed_operator_div (struct speed_params *s);
double speed_operator_mod (struct speed_params *s);

double speed_udiv_qrnnd _PROTO ((struct speed_params *s));
double speed_udiv_qrnnd_preinv _PROTO ((struct speed_params *s));
double speed_udiv_qrnnd_preinv2norm _PROTO ((struct speed_params *s));
double speed_udiv_qrnnd_c _PROTO ((struct speed_params *s));
double speed_umul_ppmm _PROTO ((struct speed_params *s));


/* Prototypes for other routines */

/* low 32-bits in p[0], high 32-bits in p[1] */
void speed_cyclecounter _PROTO ((unsigned p[2]));

/* In i386 gcc -fPIC, ebx is a fixed register and can't be declared a dummy
   output or a clobber for the cpuid, hence an explicit save and restore.  A
   clobber as such doesn't provoke an error unfortunately (gcc 3.0), so use
   the dummy output style in non-PIC, so there's an error if somehow -fPIC
   is used without a -DPIC to tell us about it.  */
#if defined(__GNUC__) && ! defined (NO_ASM)     \
  && (defined (__i386__) || defined (__i486__))
#ifdef PIC
#define speed_cyclecounter(p)                                           \
  do {                                                                  \
    int  __speed_cyclecounter__save_ebx;                                \
    int  __speed_cyclecounter__dummy;                                   \
    __asm__ __volatile__ ("movl %%ebx, %1\n"                            \
                          "cpuid\n"                                     \
                          "movl %1, %%ebx\n"                            \
                          "rdtsc"                                       \
                          : "=a"   ((p)[0]),                            \
                            "=&rm" (__speed_cyclecounter__save_ebx),    \
                            "=c"   (__speed_cyclecounter__dummy),       \
                            "=d"   ((p)[1]));                           \
  } while (0)
#else
#define speed_cyclecounter(p)                                           \
  do {                                                                  \
    int  __speed_cyclecounter__dummy1;                                  \
    int  __speed_cyclecounter__dummy2;                                  \
    __asm__ __volatile__ ("cpuid\n"                                     \
                          "rdtsc"                                       \
                          : "=a" ((p)[0]),                              \
                            "=b" (__speed_cyclecounter__dummy1),        \
                            "=c" (__speed_cyclecounter__dummy2),        \
                            "=d" ((p)[1]));                             \
  } while (0)
#endif
#endif

double speed_cyclecounter_diff _PROTO ((const unsigned end[2],
                                        const unsigned start[2]));
int gettimeofday_microseconds_p _PROTO ((void));
int getrusage_microseconds_p _PROTO ((void));
int cycles_works_p _PROTO ((void));
long clk_tck _PROTO ((void));

int double_cmp_ptr _PROTO ((const double *p, const double *q));
void pentium_wbinvd _PROTO ((void));
typedef int (*qsort_function_t) _PROTO ((const void *, const void *));

void noop _PROTO ((void));
void noop_1 _PROTO ((mp_limb_t n));
void noop_wxs _PROTO ((mp_ptr wp, mp_srcptr xp, mp_size_t size));
void noop_wxys _PROTO ((mp_ptr wp, mp_srcptr xp, mp_srcptr yp,
                        mp_size_t size));
void mpn_cache_fill _PROTO ((mp_srcptr ptr, mp_size_t size));
void mpn_cache_fill_dummy _PROTO ((mp_limb_t n));
mp_ptr speed_tmp_alloc_adjust _PROTO ((void *ptr, mp_size_t align));
void speed_cache_fill _PROTO ((struct speed_params *s));
void speed_operand_src _PROTO ((struct speed_params *s,
                                mp_ptr ptr, mp_size_t size));
void speed_operand_dst _PROTO ((struct speed_params *s,
                                mp_ptr ptr, mp_size_t size));

extern int  speed_option_addrs;
extern int  speed_option_verbose;
void speed_option_set _PROTO((const char *s));

mp_limb_t mpn_divrem_1_div _PROTO ((mp_ptr qp, mp_size_t xsize,
                                    mp_srcptr ap, mp_size_t size,
                                    mp_limb_t d));
mp_limb_t mpn_divrem_1_inv _PROTO ((mp_ptr qp, mp_size_t xsize,
                                    mp_srcptr ap, mp_size_t size,
                                    mp_limb_t d));
mp_limb_t mpn_divrem_2_div _PROTO ((mp_ptr qp, mp_size_t qxn,
                                    mp_ptr np, mp_size_t nsize,
                                    mp_srcptr dp));
mp_limb_t mpn_divrem_2_inv _PROTO ((mp_ptr qp, mp_size_t qxn,
                                    mp_ptr np, mp_size_t nsize,
                                    mp_srcptr dp));

int mpn_jacobi_base_1 _PROTO ((mp_limb_t a, mp_limb_t b, int result_bit1));
int mpn_jacobi_base_2 _PROTO ((mp_limb_t a, mp_limb_t b, int result_bit1));
int mpn_jacobi_base_3 _PROTO ((mp_limb_t a, mp_limb_t b, int result_bit1));

mp_limb_t mpn_mod_1_div _PROTO ((mp_srcptr ap, mp_size_t size, mp_limb_t d)) __GMP_ATTRIBUTE_PURE;
mp_limb_t mpn_mod_1_inv _PROTO ((mp_srcptr ap, mp_size_t size, mp_limb_t d)) __GMP_ATTRIBUTE_PURE;

mp_size_t mpn_gcd_binary
  _PROTO ((mp_ptr gp, mp_ptr up, mp_size_t usize, mp_ptr vp, mp_size_t vsize));
mp_size_t mpn_gcdext_one_double
  _PROTO ((mp_ptr gp, mp_ptr s0p, mp_size_t *s0size,
           mp_ptr up, mp_size_t size, mp_ptr vp, mp_size_t vsize));
mp_size_t mpn_gcdext_one_single
  _PROTO ((mp_ptr gp, mp_ptr s0p, mp_size_t *s0size,
           mp_ptr up, mp_size_t size, mp_ptr vp, mp_size_t vsize));
mp_size_t mpn_gcdext_single
  _PROTO ((mp_ptr gp, mp_ptr s0p, mp_size_t *s0size,
           mp_ptr up, mp_size_t size, mp_ptr vp, mp_size_t vsize));
mp_size_t mpn_gcdext_double
  _PROTO ((mp_ptr gp, mp_ptr s0p, mp_size_t *s0size,
           mp_ptr up, mp_size_t size, mp_ptr vp, mp_size_t vsize));

mp_limb_t mpn_sb_divrem_mn_div _PROTO ((mp_ptr qp,
                                        mp_ptr np,    mp_size_t nsize,
                                        mp_srcptr dp, mp_size_t dsize));
mp_limb_t mpn_sb_divrem_mn_inv _PROTO ((mp_ptr qp,
                                        mp_ptr np,    mp_size_t nsize,
                                        mp_srcptr dp, mp_size_t dsize));

mp_size_t mpn_set_str_basecase _PROTO ((mp_ptr, const unsigned char *, size_t, int));
mp_size_t mpn_set_str_subquad _PROTO ((mp_ptr, const unsigned char *, size_t, int));

void mpn_toom3_mul_n_open _PROTO ((mp_ptr, mp_srcptr, mp_srcptr, mp_size_t,
                                   mp_ptr));
void mpn_toom3_sqr_n_open _PROTO ((mp_ptr, mp_srcptr, mp_size_t, mp_ptr));
void mpn_toom3_mul_n_mpn _PROTO ((mp_ptr, mp_srcptr, mp_srcptr, mp_size_t,
                                  mp_ptr));
void mpn_toom3_sqr_n_mpn _PROTO((mp_ptr, mp_srcptr, mp_size_t, mp_ptr));

void mpz_powm_mod _PROTO ((mpz_ptr res, mpz_srcptr base, mpz_srcptr e,
                           mpz_srcptr mod));
void mpz_powm_redc _PROTO ((mpz_ptr res, mpz_srcptr base, mpz_srcptr e,
                            mpz_srcptr mod));
void redc _PROTO ((mp_ptr cp, mp_srcptr mp, mp_size_t n, mp_limb_t Nprim,
                   mp_ptr tp));

int speed_routine_count_zeros_setup _PROTO ((struct speed_params *s,
                                             mp_ptr xp, int leading,
                                             int zero));

/* The measuring routines use these big macros to save duplication for
   similar forms.  They also get used for some automatically generated
   measuring of new implementations of functions.

   Having something like SPEED_ROUTINE_BINARY_N as a subroutine accepting a
   function pointer is considered undesirable since it's not the way a
   normal application will be calling, and some processors might do
   different things with an indirect call, like not branch predicting, or
   doing a full pipe flush.  At least some of the "functions" measured are
   actually macros too.

   The net effect is to bloat the object code, possibly in a big way, but
   only what's being measured is being run, so that doesn't matter.

   Note that if a called function is __GMP_ATTRIBUTE_PURE or
   ATTRIBUTE_CONST, as for example mpn_mod_1, then the return value should
   be used in some way, to stop gcc 3 and up from discarding the calls.  See
   SPEED_ROUTINE_MPN_MOD_CALL for instance.  */


#define SPEED_RESTRICT_COND(cond)   if (!(cond)) return -1.0;

/* For mpn_copy or similar. */
#define SPEED_ROUTINE_MPN_COPY(function)		\
  {                                                     \
    mp_ptr    wp;                                       \
    unsigned  i;                                        \
    double    t;                                        \
    TMP_DECL (marker);                                  \
                                                        \
    SPEED_RESTRICT_COND (s->size >= 0);                 \
                                                        \
    TMP_MARK (marker);                                  \
    wp = SPEED_TMP_ALLOC_LIMBS (s->size, s->align_wp);  \
                                                        \
    speed_operand_src (s, s->xp, s->size);              \
    speed_operand_dst (s, wp, s->size);                 \
    speed_cache_fill (s);                               \
                                                        \
    speed_starttime ();                                 \
    i = s->reps;                                        \
    do                                                  \
      function (wp, s->xp, s->size);			\
    while (--i != 0);                                   \
    t = speed_endtime ();                               \
                                                        \
    TMP_FREE (marker);                                  \
    return t;                                           \
  }  

#define SPEED_ROUTINE_MPN_COPYC(function)		\
  {                                                     \
    mp_ptr    wp;                                       \
    unsigned  i;                                        \
    double    t;                                        \
    TMP_DECL (marker);                                  \
                                                        \
    SPEED_RESTRICT_COND (s->size >= 0);                 \
                                                        \
    TMP_MARK (marker);                                  \
    wp = SPEED_TMP_ALLOC_LIMBS (s->size, s->align_wp);  \
                                                        \
    speed_operand_src (s, s->xp, s->size);              \
    speed_operand_dst (s, wp, s->size);                 \
    speed_cache_fill (s);                               \
                                                        \
    speed_starttime ();                                 \
    i = s->reps;                                        \
    do                                                  \
      function (wp, s->xp, s->size, 0);			\
    while (--i != 0);                                   \
    t = speed_endtime ();                               \
                                                        \
    TMP_FREE (marker);                                  \
    return t;                                           \
  }  

/* s->size is still in limbs, and it's limbs which are copied, but
   "function" takes a size in bytes not limbs.  */
#define SPEED_ROUTINE_MPN_COPY_BYTES(function)			\
  {								\
    mp_ptr    wp;						\
    unsigned  i;						\
    double    t;						\
    TMP_DECL (marker);						\
								\
    SPEED_RESTRICT_COND (s->size >= 0);				\
								\
    TMP_MARK (marker);						\
    wp = SPEED_TMP_ALLOC_LIMBS (s->size, s->align_wp);		\
								\
    speed_operand_src (s, s->xp, s->size);			\
    speed_operand_dst (s, wp, s->size);				\
    speed_cache_fill (s);					\
								\
    speed_starttime ();						\
    i = s->reps;						\
    do								\
      function (wp, s->xp, s->size * BYTES_PER_MP_LIMB);	\
    while (--i != 0);						\
    t = speed_endtime ();					\
								\
    TMP_FREE (marker);						\
    return t;							\
  }  


/* For mpn_add_n, mpn_sub_n, or similar. */
#define SPEED_ROUTINE_MPN_BINARY_N_CALL(call)           \
  {                                                     \
    mp_ptr     wp;                                      \
    mp_ptr     xp, yp;                                  \
    unsigned   i;                                       \
    double     t;                                       \
    TMP_DECL (marker);                                  \
                                                        \
    SPEED_RESTRICT_COND (s->size >= 1);                 \
                                                        \
    TMP_MARK (marker);                                  \
    wp = SPEED_TMP_ALLOC_LIMBS (s->size, s->align_wp);  \
                                                        \
    xp = s->xp;                                         \
    yp = s->yp;                                         \
                                                        \
    if (s->r == 0)      ;                               \
    else if (s->r == 1) { xp = wp;          }           \
    else if (s->r == 2) {          yp = wp; }           \
    else if (s->r == 3) { xp = wp; yp = wp; }           \
    else if (s->r == 4) {     yp = xp;      }           \
    else                {                               \
      TMP_FREE (marker);                                \
      return -1.0;                                      \
    }                                                   \
                                                        \
    /* initialize wp if operand overlap */              \
    if (xp == wp || yp == wp)                           \
      MPN_COPY (wp, s->xp, s->size);                    \
                                                        \
    speed_operand_src (s, xp, s->size);                 \
    speed_operand_src (s, yp, s->size);                 \
    speed_operand_dst (s, wp, s->size);                 \
    speed_cache_fill (s);                               \
                                                        \
    speed_starttime ();                                 \
    i = s->reps;                                        \
    do                                                  \
      call;                                             \
    while (--i != 0);                                   \
    t = speed_endtime ();                               \
                                                        \
    TMP_FREE (marker);                                  \
    return t;                                           \
  }  

#define SPEED_ROUTINE_MPN_BINARY_N(function) \
   SPEED_ROUTINE_MPN_BINARY_N_CALL ((*function) (wp, xp, yp, s->size))

#define SPEED_ROUTINE_MPN_BINARY_NC(function) \
   SPEED_ROUTINE_MPN_BINARY_N_CALL ((*function) (wp, xp, yp, s->size, 0))


/* For mpn_lshift, mpn_rshift, mpn_mul_1, with r, or similar. */
#define SPEED_ROUTINE_MPN_UNARY_1_CALL(call)            \
  {                                                     \
    mp_ptr    wp;                                       \
    unsigned  i;                                        \
    double    t;                                        \
    TMP_DECL (marker);                                  \
                                                        \
    SPEED_RESTRICT_COND (s->size >= 1);                 \
                                                        \
    TMP_MARK (marker);                                  \
    wp = SPEED_TMP_ALLOC_LIMBS (s->size, s->align_wp);  \
                                                        \
    speed_operand_src (s, s->xp, s->size);              \
    speed_operand_dst (s, wp, s->size);                 \
    speed_cache_fill (s);                               \
                                                        \
    speed_starttime ();                                 \
    i = s->reps;                                        \
    do                                                  \
      call;                                             \
    while (--i != 0);                                   \
    t = speed_endtime ();                               \
                                                        \
    TMP_FREE (marker);                                  \
    return t;                                           \
  }  

#define SPEED_ROUTINE_MPN_UNARY_1(function) \
  SPEED_ROUTINE_MPN_UNARY_1_CALL ((*function) (wp, s->xp, s->size, s->r))

#define SPEED_ROUTINE_MPN_UNARY_1C(function) \
  SPEED_ROUTINE_MPN_UNARY_1_CALL ((*function) (wp, s->xp, s->size, s->r, 0))

/* FIXME: wp is uninitialized here, should start it off from xp */
#define SPEED_ROUTINE_MPN_UNARY_1_INPLACE(function) \
  SPEED_ROUTINE_MPN_UNARY_1_CALL ((*function) (wp, wp, s->size, s->r))

#define SPEED_ROUTINE_MPN_DIVEXACT_1(function)                            \
  SPEED_ROUTINE_MPN_UNARY_1_CALL ((*function) (wp, s->xp, s->size, s->r))

#define SPEED_ROUTINE_MPN_DIVREM_1(function) \
  SPEED_ROUTINE_MPN_UNARY_1_CALL ((*function) (wp, 0, s->xp, s->size, s->r))

#define SPEED_ROUTINE_MPN_DIVREM_1C(function) \
  SPEED_ROUTINE_MPN_UNARY_1_CALL ((*function) (wp, 0, s->xp, s->size, s->r, 0))

#define SPEED_ROUTINE_MPN_DIVREM_1F(function) \
  SPEED_ROUTINE_MPN_UNARY_1_CALL ((*function) (wp, s->size, s->xp, 0, s->r))

#define SPEED_ROUTINE_MPN_DIVREM_1CF(function) \
  SPEED_ROUTINE_MPN_UNARY_1_CALL ((*function) (wp, s->size, s->xp, 0, s->r, 0))


/* s->r is duplicated to form the multiplier.  Not sure if that's
   particularly useful, but at least it provides some control.  */
#define SPEED_ROUTINE_MPN_MUL_2(function)                       \
  {                                                             \
    mp_ptr     wp;                                              \
    unsigned   i;                                               \
    double     t;                                               \
    mp_limb_t  mult[2];                                         \
    TMP_DECL (marker);                                          \
                                                                \
    SPEED_RESTRICT_COND (s->size >= 1);                         \
                                                                \
    TMP_MARK (marker);                                          \
    wp = SPEED_TMP_ALLOC_LIMBS (s->size+1, s->align_wp);        \
    mult[0] = s->r;                                             \
    mult[1] = s->r;                                             \
                                                                \
    speed_operand_src (s, s->xp, s->size);                      \
    speed_operand_src (s, mult, 2);                             \
    speed_operand_dst (s, wp, s->size+1);                       \
    speed_cache_fill (s);                                       \
                                                                \
    speed_starttime ();                                         \
    i = s->reps;                                                \
    do                                                          \
      function (wp, s->xp, s->size, mult);                      \
    while (--i != 0);                                           \
    t = speed_endtime ();                                       \
                                                                \
    TMP_FREE (marker);                                          \
    return t;                                                   \
  }


#define SPEED_ROUTINE_MPN_PREINV_DIVREM_1_CALL(call)    \
  {                                                     \
    unsigned   shift;                                   \
    mp_limb_t  dinv;                                    \
                                                        \
    SPEED_RESTRICT_COND (s->size >= 0);                 \
    SPEED_RESTRICT_COND (s->r != 0);                    \
                                                        \
    count_leading_zeros (shift, s->r);                  \
    invert_limb (dinv, s->r << shift);                  \
                                                        \
    SPEED_ROUTINE_MPN_UNARY_1_CALL (call);              \
  }                                                     \

#define SPEED_ROUTINE_MPN_PREINV_DIVREM_1(function)             \
  SPEED_ROUTINE_MPN_PREINV_DIVREM_1_CALL                        \
  ((*function) (wp, 0, s->xp, s->size, s->r, dinv, shift))

/* s->size limbs worth of fraction part */
#define SPEED_ROUTINE_MPN_PREINV_DIVREM_1F(function)            \
  SPEED_ROUTINE_MPN_PREINV_DIVREM_1_CALL                        \
  ((*function) (wp, s->size, s->xp, 0, s->r, dinv, shift))


/* For mpn_lshift, mpn_rshift, mpn_mul_1, with r, or similar. */
#define SPEED_ROUTINE_MPN_UNARY_2_CALL(call)                    \
  {                                                             \
    mp_ptr     wp;                                              \
    unsigned   i;                                               \
    double     t;                                               \
    mp_limb_t  h, l;                                            \
    TMP_DECL (marker);                                          \
                                                                \
    SPEED_RESTRICT_COND (s->size >= 1);                         \
                                                                \
    TMP_MARK (marker);                                          \
    wp = SPEED_TMP_ALLOC_LIMBS (s->size+1, s->align_wp);        \
    l = s->yp[0];                                               \
    h = s->yp[1];                                               \
                                                                \
    speed_operand_src (s, s->xp, s->size);                      \
    speed_operand_dst (s, wp, s->size+1);                       \
    speed_cache_fill (s);                                       \
                                                                \
    speed_starttime ();                                         \
    i = s->reps;                                                \
    do                                                          \
      call;                                                     \
    while (--i != 0);                                           \
    t = speed_endtime ();                                       \
                                                                \
    TMP_FREE (marker);                                          \
    return t;                                                   \
  }  

#define SPEED_ROUTINE_MPN_UNARY_2(function) \
  SPEED_ROUTINE_MPN_UNARY_2_CALL ((*function) (wp, s->xp, s->size, l, h))


/* For mpn_mul_basecase, xsize=r, ysize=s->size. */
#define SPEED_ROUTINE_MPN_MUL_BASECASE(function)                \
  {                                                             \
    mp_ptr    wp;                                               \
    mp_size_t  size1;                                           \
    unsigned  i;                                                \
    double    t;                                                \
    TMP_DECL (marker);                                          \
                                                                \
    size1 = (s->r == 0 ? s->size : s->r);                       \
                                                                \
    SPEED_RESTRICT_COND (s->size >= 1);                         \
    SPEED_RESTRICT_COND (size1 >= s->size);                     \
                                                                \
    TMP_MARK (marker);                                          \
    wp = SPEED_TMP_ALLOC_LIMBS (size1 + s->size, s->align_wp);  \
                                                                \
    speed_operand_src (s, s->xp, size1);                        \
    speed_operand_src (s, s->yp, s->size);                      \
    speed_operand_dst (s, wp, size1 + s->size);                 \
    speed_cache_fill (s);                                       \
                                                                \
    speed_starttime ();                                         \
    i = s->reps;                                                \
    do                                                          \
      function (wp, s->xp, size1, s->yp, s->size);              \
    while (--i != 0);                                           \
    t = speed_endtime ();                                       \
                                                                \
    TMP_FREE (marker);                                          \
    return t;                                                   \
  }  


#define SPEED_ROUTINE_MPN_MUL_N_CALL(call)                      \
  {                                                             \
    mp_ptr    wp;                                               \
    unsigned  i;                                                \
    double    t;                                                \
    TMP_DECL (marker);                                          \
                                                                \
    SPEED_RESTRICT_COND (s->size >= 1);                         \
                                                                \
    TMP_MARK (marker);                                          \
    wp = SPEED_TMP_ALLOC_LIMBS (2*s->size, s->align_wp);        \
                                                                \
    speed_operand_src (s, s->xp, s->size);                      \
    speed_operand_src (s, s->yp, s->size);                      \
    speed_operand_dst (s, wp, 2*s->size);                       \
    speed_cache_fill (s);                                       \
                                                                \
    speed_starttime ();                                         \
    i = s->reps;                                                \
    do                                                          \
      call;                                                     \
    while (--i != 0);                                           \
    t = speed_endtime ();                                       \
                                                                \
    TMP_FREE (marker);                                          \
    return t;                                                   \
  }  

#define SPEED_ROUTINE_MPN_MUL_N(function) \
  SPEED_ROUTINE_MPN_MUL_N_CALL (function (wp, s->xp, s->yp, s->size));


#define SPEED_ROUTINE_MPN_MUL_N_TSPACE(call, tsize, minsize)    \
  {                                                             \
    mp_ptr    wp, tspace;                                       \
    unsigned  i;                                                \
    double    t;                                                \
    TMP_DECL (marker);                                          \
                                                                \
    SPEED_RESTRICT_COND (s->size >= minsize);                   \
                                                                \
    TMP_MARK (marker);                                          \
    wp = SPEED_TMP_ALLOC_LIMBS (2*s->size, s->align_wp);        \
    tspace = SPEED_TMP_ALLOC_LIMBS (tsize, s->align_wp2);       \
                                                                \
    speed_operand_src (s, s->xp, s->size);                      \
    speed_operand_src (s, s->yp, s->size);                      \
    speed_operand_dst (s, wp, 2*s->size);                       \
    speed_operand_dst (s, tspace, tsize);                       \
    speed_cache_fill (s);                                       \
                                                                \
    speed_starttime ();                                         \
    i = s->reps;                                                \
    do                                                          \
      call;                                                     \
    while (--i != 0);                                           \
    t = speed_endtime ();                                       \
                                                                \
    TMP_FREE (marker);                                          \
    return t;                                                   \
  }  

#define SPEED_ROUTINE_MPN_KARA_MUL_N(function)          \
  SPEED_ROUTINE_MPN_MUL_N_TSPACE                        \
    (function (wp, s->xp, s->xp, s->size, tspace),      \
     MPN_KARA_MUL_N_TSIZE (s->size),                    \
     MPN_KARA_MUL_N_MINSIZE)

#define SPEED_ROUTINE_MPN_TOOM3_MUL_N(function)         \
  SPEED_ROUTINE_MPN_MUL_N_TSPACE                        \
    (function (wp, s->xp, s->yp, s->size, tspace),      \
     MPN_TOOM3_MUL_N_TSIZE (s->size),                   \
     MPN_TOOM3_MUL_N_MINSIZE)


#define SPEED_ROUTINE_MPN_SQR_CALL(call)                        \
  {                                                             \
    mp_ptr    wp;                                               \
    unsigned  i;                                                \
    double    t;                                                \
    TMP_DECL (marker);                                          \
                                                                \
    SPEED_RESTRICT_COND (s->size >= 1);                         \
                                                                \
    TMP_MARK (marker);                                          \
    wp = SPEED_TMP_ALLOC_LIMBS (2*s->size, s->align_wp);        \
                                                                \
    speed_operand_src (s, s->xp, s->size);                      \
    speed_operand_dst (s, wp, 2*s->size);                       \
    speed_cache_fill (s);                                       \
                                                                \
    speed_starttime ();                                         \
    i = s->reps;                                                \
    do                                                          \
      call;                                                     \
    while (--i != 0);                                           \
    t = speed_endtime ();                                       \
                                                                \
    TMP_FREE (marker);                                          \
    return t;                                                   \
  }  

#define SPEED_ROUTINE_MPN_SQR(function) \
  SPEED_ROUTINE_MPN_SQR_CALL (function (wp, s->xp, s->size))

#define SPEED_ROUTINE_MPN_SQR_DIAGONAL(function) \
  SPEED_ROUTINE_MPN_SQR (function)


#define SPEED_ROUTINE_MPN_SQR_TSPACE(call, tsize, minsize)      \
  {                                                             \
    mp_ptr    wp, tspace;                                       \
    unsigned  i;                                                \
    double    t;                                                \
    TMP_DECL (marker);                                          \
                                                                \
    SPEED_RESTRICT_COND (s->size >= minsize);                   \
                                                                \
    TMP_MARK (marker);                                          \
    wp = SPEED_TMP_ALLOC_LIMBS (2*s->size, s->align_wp);        \
    tspace = SPEED_TMP_ALLOC_LIMBS (tsize, s->align_wp2);       \
                                                                \
    speed_operand_src (s, s->xp, s->size);                      \
    speed_operand_dst (s, wp, 2*s->size);                       \
    speed_operand_dst (s, tspace, tsize);                       \
    speed_cache_fill (s);                                       \
                                                                \
    speed_starttime ();                                         \
    i = s->reps;                                                \
    do                                                          \
      call;                                                     \
    while (--i != 0);                                           \
    t = speed_endtime ();                                       \
                                                                \
    TMP_FREE (marker);                                          \
    return t;                                                   \
  }  

#define SPEED_ROUTINE_MPN_KARA_SQR_N(function)                          \
  SPEED_ROUTINE_MPN_SQR_TSPACE (function (wp, s->xp, s->size, tspace),  \
                                MPN_KARA_SQR_N_TSIZE (s->size),         \
                                MPN_KARA_SQR_N_MINSIZE)

#define SPEED_ROUTINE_MPN_TOOM3_SQR_N(function)                         \
  SPEED_ROUTINE_MPN_SQR_TSPACE (function (wp, s->xp, s->size, tspace),  \
                                MPN_TOOM3_SQR_N_TSIZE (s->size),        \
                                MPN_TOOM3_SQR_N_MINSIZE)


#define SPEED_ROUTINE_MPN_MOD_CALL(call)        \
  {                                             \
    unsigned   i;                               \
    mp_limb_t  dummy = 0;                       \
                                                \
    SPEED_RESTRICT_COND (s->size >= 0);         \
                                                \
    speed_operand_src (s, s->xp, s->size);      \
    speed_cache_fill (s);                       \
                                                \
    speed_starttime ();                         \
    i = s->reps;                                \
    do                                          \
      dummy += call;                            \
    while (--i != 0);                           \
                                                \
    noop_1 (dummy);                             \
    return speed_endtime ();                    \
  }  

#define SPEED_ROUTINE_MPN_MOD_1(function)  \
   SPEED_ROUTINE_MPN_MOD_CALL ((*function) (s->xp, s->size, s->r))

#define SPEED_ROUTINE_MPN_MOD_1C(function) \
   SPEED_ROUTINE_MPN_MOD_CALL ((*function)(s->xp, s->size, s->r, CNST_LIMB(0)))

#define SPEED_ROUTINE_MPN_MODEXACT_1_ODD(function) \
  SPEED_ROUTINE_MPN_MOD_CALL (function (s->xp, s->size, s->r));

#define SPEED_ROUTINE_MPN_MODEXACT_1C_ODD(function) \
  SPEED_ROUTINE_MPN_MOD_CALL (function (s->xp, s->size, s->r, CNST_LIMB(0)));

#define SPEED_ROUTINE_MPN_MOD_34LSUB1(function)  \
   SPEED_ROUTINE_MPN_MOD_CALL ((*function) (s->xp, s->size))

#define SPEED_ROUTINE_MPN_PREINV_MOD_1(function)        \
  {                                                     \
    unsigned   i;                                       \
    mp_limb_t  inv;                                     \
    mp_limb_t  dummy = 0;                               \
                                                        \
    SPEED_RESTRICT_COND (s->size >= 0);                 \
    SPEED_RESTRICT_COND (s->r & GMP_LIMB_HIGHBIT);     \
                                                        \
    invert_limb (inv, s->r);                            \
    speed_operand_src (s, s->xp, s->size);              \
    speed_cache_fill (s);                               \
                                                        \
    speed_starttime ();                                 \
    i = s->reps;                                        \
    do                                                  \
      dummy += (*function) (s->xp, s->size, s->r, inv); \
    while (--i != 0);                                   \
                                                        \
    noop_1 (dummy);                                     \
    return speed_endtime ();                            \
  }  


/* A division of 2*s->size by s->size limbs */

#define SPEED_ROUTINE_MPN_DC_DIVREM_CALL(call)                  \
  {                                                             \
    unsigned  i;                                                \
    mp_ptr    a, d, q, r;                                       \
    double    t;                                                \
    TMP_DECL (marker);                                          \
                                                                \
    SPEED_RESTRICT_COND (s->size >= 1);                         \
                                                                \
    TMP_MARK (marker);                                          \
    a = SPEED_TMP_ALLOC_LIMBS (2*s->size, s->align_xp);         \
    d = SPEED_TMP_ALLOC_LIMBS (s->size,   s->align_yp);         \
    q = SPEED_TMP_ALLOC_LIMBS (s->size+1, s->align_wp);         \
    r = SPEED_TMP_ALLOC_LIMBS (s->size,   s->align_wp2);        \
                                                                \
    MPN_COPY (a, s->xp, s->size);                               \
    MPN_COPY (a+s->size, s->xp, s->size);                       \
                                                                \
    MPN_COPY (d, s->yp, s->size);                               \
                                                                \
    /* normalize the data */                                    \
    d[s->size-1] |= GMP_LIMB_HIGHBIT;                          \
    a[2*s->size-1] = d[s->size-1] - 1;                          \
                                                                \
    speed_operand_src (s, a, 2*s->size);                        \
    speed_operand_src (s, d, s->size);                          \
    speed_operand_dst (s, q, s->size+1);                        \
    speed_operand_dst (s, r, s->size);                          \
    speed_cache_fill (s);                                       \
                                                                \
    speed_starttime ();                                         \
    i = s->reps;                                                \
    do                                                          \
      call;                                                     \
    while (--i != 0);                                           \
    t = speed_endtime ();                                       \
                                                                \
    TMP_FREE (marker);                                          \
    return t;                                                   \
  }  

#define SPEED_ROUTINE_MPN_DC_DIVREM_N(function) \
  SPEED_ROUTINE_MPN_DC_DIVREM_CALL((*function) (q, a, d, s->size))

#define SPEED_ROUTINE_MPN_DC_DIVREM_SB(function)        \
  SPEED_ROUTINE_MPN_DC_DIVREM_CALL                      \
    ((*function) (q, a, 2*s->size, d, s->size))

#define SPEED_ROUTINE_MPN_DC_TDIV_QR(function)          \
  SPEED_ROUTINE_MPN_DC_DIVREM_CALL                      \
    ((*function) (q, r, 0, a, 2*s->size, d, s->size))


/* A division of s->size by 3 limbs */

#define SPEED_ROUTINE_MPN_SB_DIVREM_M3(function)        \
  {                                                     \
    unsigned   i;                                       \
    mp_ptr     a, d, q;                                 \
    mp_size_t  qsize;                                   \
    double     t;                                       \
    TMP_DECL (marker);                                  \
                                                        \
    SPEED_RESTRICT_COND (s->size >= 3);                 \
                                                        \
    TMP_MARK (marker);                                  \
    a = SPEED_TMP_ALLOC_LIMBS (s->size, s->align_xp);   \
                                                        \
    d = SPEED_TMP_ALLOC_LIMBS (3, s->align_yp);         \
    MPN_COPY (d, s->yp, 3);                             \
    d[2] |= GMP_LIMB_HIGHBIT;                          \
                                                        \
    qsize = s->size - 3;                                \
    q = SPEED_TMP_ALLOC_LIMBS (qsize, s->align_wp);     \
                                                        \
    speed_operand_dst (s, a, s->size);                  \
    speed_operand_src (s, d, 3);                        \
    speed_operand_dst (s, q, qsize);                    \
    speed_cache_fill (s);                               \
                                                        \
    speed_starttime ();                                 \
    i = s->reps;                                        \
    do                                                  \
      {                                                 \
        MPN_COPY (a, s->xp, s->size);                   \
        function (q, a, s->size, d, 3);                 \
      }                                                 \
    while (--i != 0);                                   \
    t = speed_endtime ();                               \
                                                        \
    TMP_FREE (marker);                                  \
    return t;                                           \
  }  


/* A remainder 2*s->size by s->size limbs */

#define SPEED_ROUTINE_MPZ_MOD(function)                         \
  {                                                             \
    unsigned   i;                                               \
    mpz_t      a, d, r;                                         \
                                                                \
    SPEED_RESTRICT_COND (s->size >= 1);                         \
                                                                \
    mpz_init_set_n (d, s->yp, s->size);                         \
                                                                \
    /* high part less than d, low part a duplicate copied in */ \
    mpz_init_set_n (a, s->xp, s->size);                         \
    mpz_mod (a, a, d);                                          \
    mpz_mul_2exp (a, a, BITS_PER_MP_LIMB * s->size);            \
    MPN_COPY (PTR(a), s->xp, s->size);                          \
                                                                \
    mpz_init (r);                                               \
                                                                \
    speed_operand_src (s, PTR(a), SIZ(a));                      \
    speed_operand_src (s, PTR(d), SIZ(d));                      \
    speed_cache_fill (s);                                       \
                                                                \
    speed_starttime ();                                         \
    i = s->reps;                                                \
    do                                                          \
      function (r, a, d);                                       \
    while (--i != 0);                                           \
    return speed_endtime ();                                    \
  }  


#define SPEED_ROUTINE_REDC(function)                            \
  {                                                             \
    unsigned   i;                                               \
    mp_ptr     cp, mp, tp, ap;                                  \
    mp_limb_t  Nprim;                                           \
    double     t;                                               \
    TMP_DECL (marker);                                          \
                                                                \
    SPEED_RESTRICT_COND (s->size >= 1);                         \
                                                                \
    TMP_MARK (marker);                                          \
    ap = SPEED_TMP_ALLOC_LIMBS (2*s->size+1, s->align_xp);      \
    mp = SPEED_TMP_ALLOC_LIMBS (s->size,     s->align_yp);      \
    cp = SPEED_TMP_ALLOC_LIMBS (s->size,     s->align_wp);      \
    tp = SPEED_TMP_ALLOC_LIMBS (2*s->size+1, s->align_wp2);     \
                                                                \
    MPN_COPY (ap,         s->xp, s->size);                      \
    MPN_COPY (ap+s->size, s->xp, s->size);                      \
                                                                \
    /* modulus must be odd */                                   \
    MPN_COPY (mp, s->yp, s->size);                              \
    mp[0] |= 1;                                                 \
    modlimb_invert (Nprim, mp[0]);                              \
                                                                \
    speed_operand_src (s, ap, 2*s->size+1);                     \
    speed_operand_dst (s, tp, 2*s->size+1);                     \
    speed_operand_src (s, mp, s->size);                         \
    speed_operand_dst (s, cp, s->size);                         \
    speed_cache_fill (s);                                       \
                                                                \
    speed_starttime ();                                         \
    i = s->reps;                                                \
    do {                                                        \
      MPN_COPY (tp, ap, 2*s->size);                             \
      function (cp, mp, s->size, Nprim, tp);                    \
    } while (--i != 0);                                         \
    t = speed_endtime ();                                       \
                                                                \
    TMP_FREE (marker);                                          \
    return t;                                                   \
  }  


#define SPEED_ROUTINE_MPN_POPCOUNT(function)    \
  {                                             \
    unsigned       i;                           \
    unsigned long  dummy = 0;                   \
                                                \
    SPEED_RESTRICT_COND (s->size >= 1);         \
                                                \
    speed_operand_src (s, s->xp, s->size);      \
    speed_cache_fill (s);                       \
                                                \
    speed_starttime ();                         \
    i = s->reps;                                \
    do                                          \
      dummy += function (s->xp, s->size);       \
    while (--i != 0);                           \
                                                \
    noop_1 ((mp_limb_t) dummy);                 \
    return speed_endtime ();                    \
  }  

#define SPEED_ROUTINE_MPN_HAMDIST(function)             \
  {                                                     \
    unsigned       i;                                   \
    unsigned long  dummy = 0;                           \
                                                        \
    SPEED_RESTRICT_COND (s->size >= 1);                 \
                                                        \
    speed_operand_src (s, s->xp, s->size);              \
    speed_operand_src (s, s->yp, s->size);              \
    speed_cache_fill (s);                               \
                                                        \
    speed_starttime ();                                 \
    i = s->reps;                                        \
    do                                                  \
      dummy += function (s->xp, s->yp, s->size);        \
    while (--i != 0);                                   \
                                                        \
    noop_1 ((mp_limb_t) dummy);                         \
    return speed_endtime ();                            \
  }  


#define SPEED_ROUTINE_MPZ_UI(function)  \
  {                                     \
    mpz_t     z;                        \
    unsigned  i;                        \
    double    t;                        \
                                        \
    SPEED_RESTRICT_COND (s->size >= 0); \
                                        \
    mpz_init (z);                       \
                                        \
    speed_starttime ();                 \
    i = s->reps;                        \
    do                                  \
      function (z, s->size);            \
    while (--i != 0);                   \
    t = speed_endtime ();               \
                                        \
    mpz_clear (z);                      \
    return t;                           \
  }  

#define SPEED_ROUTINE_MPZ_FAC_UI(function)    SPEED_ROUTINE_MPZ_UI(function)
#define SPEED_ROUTINE_MPZ_FIB_UI(function)    SPEED_ROUTINE_MPZ_UI(function)
#define SPEED_ROUTINE_MPZ_LUCNUM_UI(function) SPEED_ROUTINE_MPZ_UI(function)


#define SPEED_ROUTINE_MPZ_2_UI(function)        \
  {                                             \
    mpz_t     z, z2;                            \
    unsigned  i;                                \
    double    t;                                \
                                                \
    SPEED_RESTRICT_COND (s->size >= 0);         \
                                                \
    mpz_init (z);                               \
    mpz_init (z2);                              \
                                                \
    speed_starttime ();                         \
    i = s->reps;                                \
    do                                          \
      function (z, z2, s->size);                \
    while (--i != 0);                           \
    t = speed_endtime ();                       \
                                                \
    mpz_clear (z);                              \
    mpz_clear (z2);                             \
    return t;                                   \
  }  

#define SPEED_ROUTINE_MPZ_FIB2_UI(function)    SPEED_ROUTINE_MPZ_2_UI(function)
#define SPEED_ROUTINE_MPZ_LUCNUM2_UI(function) SPEED_ROUTINE_MPZ_2_UI(function)


#define SPEED_ROUTINE_MPN_FIB2_UI(function)             \
  {                                                     \
    mp_ptr     fp, f1p;                                 \
    mp_size_t  alloc;                                   \
    unsigned   i;                                       \
    double     t;                                       \
    TMP_DECL (marker);                                  \
                                                        \
    SPEED_RESTRICT_COND (s->size >= 0);                 \
                                                        \
    TMP_MARK (marker);                                  \
    alloc = MPN_FIB2_SIZE (s->size);                    \
    fp  = SPEED_TMP_ALLOC_LIMBS (alloc, s->align_xp);   \
    f1p = SPEED_TMP_ALLOC_LIMBS (alloc, s->align_yp);   \
                                                        \
    speed_starttime ();                                 \
    i = s->reps;                                        \
    do                                                  \
      function (fp, f1p, s->size);                      \
    while (--i != 0);                                   \
    t = speed_endtime ();                               \
                                                        \
    TMP_FREE (marker);                                  \
    return t;                                           \
  }  



/* Calculate b^e mod m for random b and m of s->size limbs and random e of 6
   limbs.  m is forced to odd so that redc can be used.  e is limited in
   size so the calculation doesn't take too long. */
#define SPEED_ROUTINE_MPZ_POWM(function)        \
  {                                             \
    mpz_t     r, b, e, m;                       \
    unsigned  i;                                \
    double    t;                                \
                                                \
    SPEED_RESTRICT_COND (s->size >= 1);         \
                                                \
    mpz_init (r);                               \
    mpz_init_set_n (b, s->xp, s->size);         \
    mpz_init_set_n (m, s->yp, s->size);         \
    mpz_setbit (m, 0);  /* force m to odd */    \
    mpz_init_set_n (e, s->xp_block, 6);         \
                                                \
    speed_starttime ();                         \
    i = s->reps;                                \
    do                                          \
      function (r, b, e, m);                    \
    while (--i != 0);                           \
    t = speed_endtime ();                       \
                                                \
    mpz_clear (r);                              \
    mpz_clear (b);                              \
    mpz_clear (e);                              \
    mpz_clear (m);                              \
    return t;                                   \
  }  

/* (m-2)^0xAAAAAAAA mod m */
#define SPEED_ROUTINE_MPZ_POWM_UI(function)             \
  {                                                     \
    mpz_t          r, b, m;                             \
    unsigned long  e = (~ (unsigned long) 0) / 3;       \
    unsigned       i;                                   \
    double         t;                                   \
                                                        \
    SPEED_RESTRICT_COND (s->size >= 1);                 \
                                                        \
    mpz_init (r);                                       \
                                                        \
    /* force m to odd */                                \
    mpz_init (m);                                       \
    mpz_set_n (m, s->xp, s->size);                      \
    PTR(m)[0] |= 1;                                     \
                                                        \
    mpz_init_set (b, m);                                \
    mpz_sub_ui (b, b, 2);                               \
/* printf ("%X\n", mpz_get_ui(m)); */                   \
    i = s->reps;                                        \
    speed_starttime ();                                 \
    do                                                  \
      function (r, b, e, m);                            \
    while (--i != 0);                                   \
    t = speed_endtime ();                               \
                                                        \
    mpz_clear (r);                                      \
    mpz_clear (b);                                      \
    mpz_clear (m);                                      \
    return t;                                           \
  }  


#define SPEED_ROUTINE_MPN_ADDSUB_CALL(call)                     \
  {                                                             \
    mp_ptr    wp, wp2, xp, yp;                                  \
    unsigned  i;                                                \
    double    t;                                                \
    TMP_DECL (marker);                                          \
                                                                \
    SPEED_RESTRICT_COND (s->size >= 0);                         \
                                                                \
    TMP_MARK (marker);                                          \
    wp  = SPEED_TMP_ALLOC_LIMBS (s->size, s->align_wp);         \
    wp2 = SPEED_TMP_ALLOC_LIMBS (s->size, s->align_wp2);        \
    xp = s->xp;                                                 \
    yp = s->yp;                                                 \
                                                                \
    if (s->r == 0)      ;                                       \
    else if (s->r == 1) { xp = wp;            }                 \
    else if (s->r == 2) {           yp = wp2; }                 \
    else if (s->r == 3) { xp = wp;  yp = wp2; }                 \
    else if (s->r == 4) { xp = wp2; yp = wp;  }                 \
    else {                                                      \
      TMP_FREE (marker);                                        \
      return -1.0;                                              \
    }                                                           \
    if (xp != s->xp) MPN_COPY (xp, s->xp, s->size);             \
    if (yp != s->yp) MPN_COPY (yp, s->yp, s->size);             \
                                                                \
    speed_operand_src (s, xp, s->size);                         \
    speed_operand_src (s, yp, s->size);                         \
    speed_operand_dst (s, wp, s->size);                         \
    speed_operand_dst (s, wp2, s->size);                        \
    speed_cache_fill (s);                                       \
                                                                \
    speed_starttime ();                                         \
    i = s->reps;                                                \
    do                                                          \
      call;                                                     \
    while (--i != 0);                                           \
    t = speed_endtime ();                                       \
                                                                \
    TMP_FREE (marker);                                          \
    return t;                                                   \
  }

#define SPEED_ROUTINE_MPN_ADDSUB_N(function)    \
  SPEED_ROUTINE_MPN_ADDSUB_CALL                 \
    (function (wp, wp2, xp, yp, s->size));

#define SPEED_ROUTINE_MPN_ADDSUB_NC(function)   \
  SPEED_ROUTINE_MPN_ADDSUB_CALL                 \
    (function (wp, wp2, xp, yp, s->size, 0));


/* Doing an Nx1 gcd with the given r. */
#define SPEED_ROUTINE_MPN_GCD_1N(function)              \
  {                                                     \
    mp_ptr    xp;                                       \
    unsigned  i;                                        \
    double    t;                                        \
    mp_limb_t dummy = 0;                                \
    TMP_DECL (marker);                                  \
                                                        \
    SPEED_RESTRICT_COND (s->size >= 1);                 \
    SPEED_RESTRICT_COND (s->r != 0);                    \
                                                        \
    TMP_MARK (marker);                                  \
    xp = SPEED_TMP_ALLOC_LIMBS (s->size, s->align_xp);  \
    MPN_COPY (xp, s->xp, s->size);                      \
    xp[0] |= refmpn_zero_p (xp, s->size);               \
                                                        \
    speed_operand_src (s, s->xp, s->size);              \
    speed_cache_fill (s);                               \
                                                        \
    speed_starttime ();                                 \
    i = s->reps;                                        \
    do                                                  \
      dummy += function (xp, s->size, s->r);            \
    while (--i != 0);                                   \
    t = speed_endtime ();                               \
                                                        \
    noop_1 (dummy);                                     \
    TMP_FREE (marker);                                  \
    return t;                                           \
  }  


/* SPEED_BLOCK_SIZE many one GCDs of s->size bits each. */

#define SPEED_ROUTINE_MPN_GCD_1_CALL(setup, call)               \
  {                                                             \
    unsigned  i, j;                                             \
    mp_ptr    px, py;                                           \
    mp_limb_t x_mask, y_mask;                                   \
    mp_limb_t dummy = 0;                                        \
    double    t;                                                \
    TMP_DECL (marker);                                          \
                                                                \
    SPEED_RESTRICT_COND (s->size >= 1);                         \
    SPEED_RESTRICT_COND (s->size <= mp_bits_per_limb);          \
                                                                \
    TMP_MARK (marker);                                          \
    px = SPEED_TMP_ALLOC_LIMBS (SPEED_BLOCK_SIZE, s->align_xp); \
    py = SPEED_TMP_ALLOC_LIMBS (SPEED_BLOCK_SIZE, s->align_yp); \
    MPN_COPY (px, s->xp_block, SPEED_BLOCK_SIZE);               \
    MPN_COPY (py, s->yp_block, SPEED_BLOCK_SIZE);               \
                                                                \
    x_mask = MP_LIMB_T_LOWBITMASK (s->size);                    \
    y_mask = MP_LIMB_T_LOWBITMASK (s->r != 0 ? s->r : s->size); \
    for (i = 0; i < SPEED_BLOCK_SIZE; i++)                      \
      {                                                         \
        px[i] &= x_mask; px[i] += (px[i] == 0);                 \
        py[i] &= y_mask; py[i] += (py[i] == 0);                 \
        setup;                                                  \
      }                                                         \
                                                                \
    speed_operand_src (s, px, SPEED_BLOCK_SIZE);                \
    speed_operand_src (s, py, SPEED_BLOCK_SIZE);                \
    speed_cache_fill (s);                                       \
                                                                \
    speed_starttime ();                                         \
    i = s->reps;                                                \
    do                                                          \
      {                                                         \
        j = SPEED_BLOCK_SIZE;                                   \
        do                                                      \
          {                                                     \
            dummy += call;                                      \
          }                                                     \
        while (--j != 0);                                       \
      }                                                         \
    while (--i != 0);                                           \
    t = speed_endtime ();                                       \
                                                                \
    TMP_FREE (marker);                                          \
                                                                \
    noop_1 (dummy);                                             \
    s->time_divisor = SPEED_BLOCK_SIZE;                         \
    return t;                                                   \
  }  

#define SPEED_ROUTINE_MPN_GCD_1(function) \
  SPEED_ROUTINE_MPN_GCD_1_CALL( , function (&px[j-1], 1, py[j-1]))

#define SPEED_ROUTINE_MPN_JACBASE(function)     \
  SPEED_ROUTINE_MPN_GCD_1_CALL                  \
    ({                                          \
       /* require x<y, y odd, y!=1 */           \
       px[i] %= py[i];                          \
       px[i] |= 1;                              \
       py[i] |= 1;                              \
       if (py[i]==1) py[i]=3;                   \
     },                                         \
     function (px[j-1], py[j-1], 0))


/* Run some GCDs of s->size limbs each.  The number of different data values
   is decreased as s->size**2, since GCD is a quadratic algorithm.
   SPEED_ROUTINE_MPN_GCD runs more times than SPEED_ROUTINE_MPN_GCDEXT
   though, because the plain gcd is about twice as fast as gcdext.  */

#define SPEED_ROUTINE_MPN_GCD_CALL(datafactor, call)            \
  {                                                             \
    unsigned  i;                                                \
    mp_size_t j, pieces, psize;                                 \
    mp_ptr    wp, wp2, xtmp, ytmp, px, py;                      \
    double    t;                                                \
    TMP_DECL (marker);                                          \
                                                                \
    SPEED_RESTRICT_COND (s->size >= 1);                         \
                                                                \
    TMP_MARK (marker);                                          \
    xtmp = SPEED_TMP_ALLOC_LIMBS (s->size+1, s->align_xp);      \
    ytmp = SPEED_TMP_ALLOC_LIMBS (s->size+1, s->align_yp);      \
    wp = SPEED_TMP_ALLOC_LIMBS (s->size+1, s->align_wp);        \
    wp2 = SPEED_TMP_ALLOC_LIMBS (s->size+1, s->align_wp2);      \
                                                                \
    pieces = SPEED_BLOCK_SIZE * datafactor / s->size / s->size; \
    pieces = MIN (pieces, SPEED_BLOCK_SIZE / s->size);          \
    pieces = MAX (pieces, 1);                                   \
                                                                \
    psize = pieces * s->size;                                   \
    px = TMP_ALLOC_LIMBS (psize);                               \
    py = TMP_ALLOC_LIMBS (psize);                               \
    MPN_COPY (px, pieces==1 ? s->xp : s->xp_block, psize);      \
    MPN_COPY (py, pieces==1 ? s->yp : s->yp_block, psize);      \
                                                                \
    /* y must be odd, x must have at least as many bits as y,   \
       high limbs must be non-zero */                           \
    for (j = 0; j < pieces; j++)                                \
      {                                                         \
        mp_ptr  x = px+j*s->size;                               \
        mp_ptr  y = py+j*s->size;                               \
        y[0] |= 1;                                              \
        if (x[s->size-1] == 0) x[s->size-1] = 1;                \
        if (y[s->size-1] == 0) y[s->size-1] = 1;                \
        x[s->size-1] = MAX (x[s->size-1], y[s->size-1]);        \
      }                                                         \
                                                                \
    speed_operand_src (s, px, psize);                           \
    speed_operand_src (s, py, psize);                           \
    speed_operand_dst (s, xtmp, s->size);                       \
    speed_operand_dst (s, ytmp, s->size);                       \
    speed_operand_dst (s, wp, s->size);                         \
    speed_cache_fill (s);                                       \
                                                                \
    speed_starttime ();                                         \
    i = s->reps;                                                \
    do                                                          \
      {                                                         \
        j = pieces;                                             \
        do                                                      \
          {                                                     \
            MPN_COPY (xtmp, px+(j-1)*s->size, s->size);         \
            MPN_COPY (ytmp, py+(j-1)*s->size, s->size);         \
            call;                                               \
          }                                                     \
        while (--j != 0);                                       \
      }                                                         \
    while (--i != 0);                                           \
    t = speed_endtime ();                                       \
                                                                \
    TMP_FREE (marker);                                          \
                                                                \
    s->time_divisor = pieces;                                   \
    return t;                                                   \
  }  

#define SPEED_ROUTINE_MPN_GCD(function) \
  SPEED_ROUTINE_MPN_GCD_CALL (8, function (wp, xtmp, s->size, ytmp, s->size))

#define SPEED_ROUTINE_MPN_GCDEXT(function)                               \
  SPEED_ROUTINE_MPN_GCD_CALL                                             \
    (4, { mp_size_t  wp2size;                                            \
          function (wp, wp2, &wp2size, xtmp, s->size, ytmp, s->size); })


#define SPEED_ROUTINE_MPN_GCDEXT_ONE(function)                          \
  {                                                                     \
    unsigned  i;                                                        \
    mp_size_t j, pieces, psize, wp2size;                                \
    mp_ptr    wp, wp2, xtmp, ytmp, px, py;                              \
    double    t;                                                        \
    TMP_DECL (marker);                                                  \
                                                                        \
    SPEED_RESTRICT_COND (s->size >= 1);                                 \
                                                                        \
    TMP_MARK (marker);                                                  \
                                                                        \
    xtmp = SPEED_TMP_ALLOC_LIMBS (s->size+1, s->align_xp);              \
    ytmp = SPEED_TMP_ALLOC_LIMBS (s->size+1, s->align_yp);              \
    MPN_COPY (xtmp, s->xp, s->size);                                    \
    MPN_COPY (ytmp, s->yp, s->size);                                    \
                                                                        \
    wp = SPEED_TMP_ALLOC_LIMBS (s->size+1, s->align_wp);                \
    wp2 = SPEED_TMP_ALLOC_LIMBS (s->size+1, s->align_wp2);              \
                                                                        \
    pieces = SPEED_BLOCK_SIZE / 3;                                      \
    psize = 3 * pieces;                                                 \
    px = TMP_ALLOC_LIMBS (psize);                                       \
    py = TMP_ALLOC_LIMBS (psize);                                       \
    MPN_COPY (px, s->xp_block, psize);                                  \
    MPN_COPY (py, s->yp_block, psize);                                  \
                                                                        \
    /* x must have at least as many bits as y,                          \
       high limbs must be non-zero */                                   \
    for (j = 0; j < pieces; j++)                                        \
      {                                                                 \
        mp_ptr  x = px+3*j;                                             \
        mp_ptr  y = py+3*j;                                             \
        x[2] += (x[2] == 0);                                            \
        y[2] += (y[2] == 0);                                            \
        if (x[2] < y[2])                                                \
          MP_LIMB_T_SWAP (x[2], y[2]);                                  \
      }                                                                 \
                                                                        \
    speed_operand_src (s, px, psize);                                   \
    speed_operand_src (s, py, psize);                                   \
    speed_operand_dst (s, xtmp, s->size);                               \
    speed_operand_dst (s, ytmp, s->size);                               \
    speed_operand_dst (s, wp, s->size);                                 \
    speed_cache_fill (s);                                               \
                                                                        \
    speed_starttime ();                                                 \
    i = s->reps;                                                        \
    do                                                                  \
      {                                                                 \
        mp_ptr  x = px;                                                 \
        mp_ptr  y = py;                                                 \
        mp_ptr  xth = &xtmp[s->size-3];                                 \
        mp_ptr  yth = &ytmp[s->size-3];                                 \
        j = pieces;                                                     \
        do                                                              \
          {                                                             \
            xth[0] = x[0], xth[1] = x[1], xth[2] = x[2];                \
            yth[0] = y[0], yth[1] = y[1], yth[2] = y[2];                \
                                                                        \
            ytmp[0] |= 1; /* y must be odd, */                          \
                                                                        \
            function (wp, wp2, &wp2size, xtmp, s->size, ytmp, s->size); \
                                                                        \
            x += 3;                                                     \
            y += 3;                                                     \
          }                                                             \
        while (--j != 0);                                               \
      }                                                                 \
    while (--i != 0);                                                   \
    t = speed_endtime ();                                               \
                                                                        \
    TMP_FREE (marker);                                                  \
                                                                        \
    s->time_divisor = pieces;                                           \
    return t;                                                           \
  }  

#define SPEED_ROUTINE_MPZ_JACOBI(function)                      \
  {                                                             \
    mpz_t     a, b;                                             \
    unsigned  i;                                                \
    mp_size_t j, pieces, psize;                                 \
    mp_ptr    px, py;                                           \
    double    t;                                                \
    int       dummy = 0;                                        \
    TMP_DECL (marker);                                          \
                                                                \
    TMP_MARK (marker);                                          \
    pieces = SPEED_BLOCK_SIZE / MAX (s->size, 1);               \
    pieces = MAX (pieces, 1);                                   \
    s->time_divisor = pieces;                                   \
                                                                \
    psize = pieces * s->size;                                   \
    px = TMP_ALLOC_LIMBS (psize);                               \
    py = TMP_ALLOC_LIMBS (psize);                               \
    MPN_COPY (px, pieces==1 ? s->xp : s->xp_block, psize);      \
    MPN_COPY (py, pieces==1 ? s->yp : s->yp_block, psize);      \
                                                                \
    for (j = 0; j < pieces; j++)                                \
      {                                                         \
        mp_ptr  x = px+j*s->size;                               \
        mp_ptr  y = py+j*s->size;                               \
                                                                \
        /* y odd */                                             \
        y[0] |= 1;                                              \
                                                                \
        /* high limbs non-zero */                               \
        if (x[s->size-1] == 0) x[s->size-1] = 1;                \
        if (y[s->size-1] == 0) y[s->size-1] = 1;                \
      }                                                         \
                                                                \
    SIZ(a) = s->size;                                           \
    SIZ(b) = s->size;                                           \
                                                                \
    speed_operand_src (s, px, psize);                           \
    speed_operand_src (s, py, psize);                           \
    speed_cache_fill (s);                                       \
                                                                \
    speed_starttime ();                                         \
    i = s->reps;                                                \
    do                                                          \
      {                                                         \
        j = pieces;                                             \
        do                                                      \
          {                                                     \
            PTR(a) = px+(j-1)*s->size;                          \
            PTR(b) = py+(j-1)*s->size;                          \
            dummy += function (a, b);                           \
          }                                                     \
        while (--j != 0);                                       \
      }                                                         \
    while (--i != 0);                                           \
    t = speed_endtime ();                                       \
                                                                \
    noop_1 ((mp_limb_t) dummy);                                 \
    TMP_FREE (marker);                                          \
    return t;                                                   \
  }  

#define SPEED_ROUTINE_MPN_DIVREM_2(function)            \
  {                                                     \
    mp_ptr    wp, xp;                                   \
    mp_limb_t yp[2];                                    \
    unsigned  i;                                        \
    double    t;                                        \
    TMP_DECL (marker);                                  \
                                                        \
    SPEED_RESTRICT_COND (s->size >= 2);                 \
                                                        \
    TMP_MARK (marker);                                  \
    xp = SPEED_TMP_ALLOC_LIMBS (s->size, s->align_xp);  \
    wp = SPEED_TMP_ALLOC_LIMBS (s->size, s->align_wp);  \
                                                        \
    /* source is destroyed */                           \
    MPN_COPY (xp, s->xp, s->size);                      \
                                                        \
    /* divisor must be normalized */                    \
    MPN_COPY (yp, s->yp_block, 2);                      \
    yp[1] |= GMP_LIMB_HIGHBIT;                         \
                                                        \
    speed_operand_src (s, xp, s->size);                 \
    speed_operand_src (s, yp, 2);                       \
    speed_operand_dst (s, wp, s->size);                 \
    speed_cache_fill (s);                               \
                                                        \
    speed_starttime ();                                 \
    i = s->reps;                                        \
    do                                                  \
      function (wp, 0, xp, s->size, yp);                \
    while (--i != 0);                                   \
    t = speed_endtime ();                               \
                                                        \
    TMP_FREE (marker);                                  \
    return t;                                           \
  }  


#define SPEED_ROUTINE_MODLIMB_INVERT(function)                  \
  {                                                             \
    unsigned   i, j;                                            \
    mp_ptr     xp;                                              \
    mp_limb_t  n = 1;                                           \
    double     t;                                               \
                                                                \
    xp = s->xp_block-1;                                         \
                                                                \
    speed_operand_src (s, s->xp_block, SPEED_BLOCK_SIZE);       \
    speed_cache_fill (s);                                       \
                                                                \
    speed_starttime ();                                         \
    i = s->reps;                                                \
    do                                                          \
      {                                                         \
        j = SPEED_BLOCK_SIZE;                                   \
        do                                                      \
          {                                                     \
            /* randomized but successively dependent */         \
            n += (xp[j] << 1);                                  \
                                                                \
            function (n, n);                                    \
          }                                                     \
        while (--j != 0);                                       \
      }                                                         \
    while (--i != 0);                                           \
    t = speed_endtime ();                                       \
                                                                \
    /* make sure the compiler won't optimize away n */          \
    noop_1 (n);                                                 \
                                                                \
    s->time_divisor = SPEED_BLOCK_SIZE;                         \
    return t;                                                   \
  }  


#define SPEED_ROUTINE_MPN_SQRTREM(function)                     \
  {                                                             \
    mp_ptr    wp, wp2;                                          \
    unsigned  i;                                                \
    double    t;                                                \
    TMP_DECL (marker);                                          \
                                                                \
    SPEED_RESTRICT_COND (s->size >= 1);                         \
                                                                \
    TMP_MARK (marker);                                          \
    wp  = SPEED_TMP_ALLOC_LIMBS (s->size, s->align_wp);         \
    wp2 = SPEED_TMP_ALLOC_LIMBS (s->size, s->align_wp2);        \
                                                                \
    speed_operand_src (s, s->xp, s->size);                      \
    speed_operand_dst (s, wp, s->size);                         \
    speed_operand_dst (s, wp2, s->size);                        \
    speed_cache_fill (s);                                       \
                                                                \
    speed_starttime ();                                         \
    i = s->reps;                                                \
    do                                                          \
      function (wp, wp2, s->xp, s->size);                       \
    while (--i != 0);                                           \
    t = speed_endtime ();                                       \
                                                                \
    TMP_FREE (marker);                                          \
    return t;                                                   \
  }  


/* s->size controls the number of limbs in the input, s->r is the base, or
   decimal by default. */
#define SPEED_ROUTINE_MPN_GET_STR(function)                             \
  {                                                                     \
    unsigned char *wp;                                                  \
    mp_size_t    wsize;                                                 \
    mp_ptr    xp;                                                       \
    int       base;                                                     \
    unsigned  i;                                                        \
    double    t;                                                        \
    TMP_DECL (marker);                                                  \
                                                                        \
    SPEED_RESTRICT_COND (s->size >= 1);                                 \
                                                                        \
    base = s->r == 0 ? 10 : s->r;                                       \
    SPEED_RESTRICT_COND (base >= 2 && base <= 256);                     \
                                                                        \
    TMP_MARK (marker);                                                  \
    xp = SPEED_TMP_ALLOC_LIMBS (s->size + 1, s->align_xp);              \
                                                                        \
    MPN_SIZEINBASE (wsize, s->xp, s->size, base);                       \
    wp = TMP_ALLOC (wsize);                                             \
                                                                        \
    /* use this during development to guard against overflowing wp */   \
    /*                                                                  \
    MPN_COPY (xp, s->xp, s->size);                                      \
    ASSERT_ALWAYS (mpn_get_str (wp, base, xp, s->size) <= wsize);       \
    */                                                                  \
                                                                        \
    speed_operand_src (s, s->xp, s->size);                              \
    speed_operand_dst (s, xp, s->size);                                 \
    speed_operand_dst (s, (mp_ptr) wp, wsize/BYTES_PER_MP_LIMB);        \
    speed_cache_fill (s);                                               \
                                                                        \
    speed_starttime ();                                                 \
    i = s->reps;                                                        \
    do                                                                  \
      {                                                                 \
        MPN_COPY (xp, s->xp, s->size);                                  \
        function (wp, base, xp, s->size);                               \
      }                                                                 \
    while (--i != 0);                                                   \
    t = speed_endtime ();                                               \
                                                                        \
    TMP_FREE (marker);                                                  \
    return t;                                                           \
  }  

/* s->size controls the number of digits in the input, s->r is the base, or
   decimal by default. */
#define SPEED_ROUTINE_MPN_SET_STR(function)                                  \
  {                                                                          \
    unsigned char *xp;                                                       \
    mp_ptr     wp;                                                           \
    mp_size_t  wsize;                                                        \
    unsigned   i;                                                            \
    int        base;                                                         \
    double     t;                                                            \
    TMP_DECL (marker);                                                       \
                                                                             \
    SPEED_RESTRICT_COND (s->size >= 1);                                      \
                                                                             \
    base = s->r == 0 ? 10 : s->r;                                            \
    SPEED_RESTRICT_COND (base >= 2 && base <= 256);                          \
                                                                             \
    TMP_MARK (marker);                                                       \
                                                                             \
    xp = TMP_ALLOC (s->size);                                                \
    for (i = 0; i < s->size; i++)                                            \
      xp[i] = s->xp[i] % base;                                               \
                                                                             \
    wsize = ((mp_size_t) (s->size / __mp_bases[base].chars_per_bit_exactly)) \
      / BITS_PER_MP_LIMB + 2;                                                \
    wp = SPEED_TMP_ALLOC_LIMBS (wsize, s->align_wp);                         \
                                                                             \
    /* use this during development to check wsize is big enough */           \
    /*                                                                       \
    ASSERT_ALWAYS (mpn_set_str (wp, xp, s->size, base) <= wsize);            \
    */                                                                       \
                                                                             \
    speed_operand_src (s, (mp_ptr) xp, s->size/BYTES_PER_MP_LIMB);           \
    speed_operand_dst (s, wp, wsize);                                        \
    speed_cache_fill (s);                                                    \
                                                                             \
    speed_starttime ();                                                      \
    i = s->reps;                                                             \
    do                                                                       \
      function (wp, xp, s->size, base);                                      \
    while (--i != 0);                                                        \
    t = speed_endtime ();                                                    \
                                                                             \
    TMP_FREE (marker);                                                       \
    return t;                                                                \
  }  


/* Run an accel gcd find_a() function over various data values.  A set of
   values is used in case some run particularly fast or slow.  The size
   parameter is ignored, the amount of data tested is fixed.  */

#define SPEED_ROUTINE_MPN_GCD_FINDA(function)                   \
  {                                                             \
    unsigned  i, j;                                             \
    mp_limb_t cp[SPEED_BLOCK_SIZE][2];                          \
    double    t;                                                \
    mp_limb_t dummy = 0;                                        \
    TMP_DECL (marker);                                          \
                                                                \
    TMP_MARK (marker);                                          \
                                                                \
    /* low must be odd, high must be non-zero */                \
    for (i = 0; i < SPEED_BLOCK_SIZE; i++)                      \
      {                                                         \
        cp[i][0] = s->xp_block[i] | 1;                          \
        cp[i][1] = s->yp_block[i] + (s->yp_block[i] == 0);      \
      }                                                         \
                                                                \
    speed_operand_src (s, &cp[0][0], 2*SPEED_BLOCK_SIZE);       \
    speed_cache_fill (s);                                       \
                                                                \
    speed_starttime ();                                         \
    i = s->reps;                                                \
    do                                                          \
      {                                                         \
        j = SPEED_BLOCK_SIZE;                                   \
        do                                                      \
          {                                                     \
            dummy += function (cp[j-1]);                        \
          }                                                     \
        while (--j != 0);                                       \
      }                                                         \
    while (--i != 0);                                           \
    t = speed_endtime ();                                       \
                                                                \
    noop_1 (dummy); /* don't let the calls go dead */           \
    TMP_FREE (marker);                                          \
                                                                \
    s->time_divisor = SPEED_BLOCK_SIZE;                         \
    return t;                                                   \
  }  


/* "call" should do "count_foo_zeros(c,n)".
   Give leading=1 if foo is leading zeros, leading=0 for trailing.
   Give zero=1 if n=0 is allowed in the call, zero=0 if not.  */

#define SPEED_ROUTINE_COUNT_ZEROS_A(leading, zero)                      \
  {                                                                     \
    mp_ptr     xp;                                                      \
    int        i, c;                                                    \
    unsigned   j;                                                       \
    mp_limb_t  n;                                                       \
    double     t;                                                       \
    TMP_DECL (marker);                                                  \
                                                                        \
    TMP_MARK (marker);                                                  \
    xp = SPEED_TMP_ALLOC_LIMBS (SPEED_BLOCK_SIZE, s->align_xp);         \
                                                                        \
    if (! speed_routine_count_zeros_setup (s, xp, leading, zero))       \
      return -1.0;                                                      \
    speed_operand_src (s, xp, SPEED_BLOCK_SIZE);                        \
    speed_cache_fill (s);                                               \
                                                                        \
    c = 0;                                                              \
    speed_starttime ();                                                 \
    j = s->reps;                                                        \
    do {                                                                \
      for (i = 0; i < SPEED_BLOCK_SIZE; i++)                            \
        {                                                               \
          n = xp[i];                                                    \
          n ^= c;                                                       \

#define SPEED_ROUTINE_COUNT_ZEROS_B()                                   \
        }                                                               \
    } while (--j != 0);                                                 \
    t = speed_endtime ();                                               \
                                                                        \
    /* don't let c go dead */                                           \
    noop_1 (c);                                                         \
                                                                        \
    s->time_divisor = SPEED_BLOCK_SIZE;                                 \
                                                                        \
    TMP_FREE (marker);                                                  \
    return t;                                                           \
  }                                                                     \

#define SPEED_ROUTINE_COUNT_ZEROS_C(call, leading, zero)        \
  do {                                                          \
    SPEED_ROUTINE_COUNT_ZEROS_A (leading, zero);                \
    call;                                                       \
    SPEED_ROUTINE_COUNT_ZEROS_B ();                             \
  } while (0)                                                   \

#define SPEED_ROUTINE_COUNT_LEADING_ZEROS_C(call,zero)  \
  SPEED_ROUTINE_COUNT_ZEROS_C (call, 1, zero)
#define SPEED_ROUTINE_COUNT_LEADING_ZEROS(fun)          \
  SPEED_ROUTINE_COUNT_ZEROS_C (fun (c, n), 1, 0)

#define SPEED_ROUTINE_COUNT_TRAILING_ZEROS_C(call,zero) \
  SPEED_ROUTINE_COUNT_ZEROS_C (call, 0, zero)
#define SPEED_ROUTINE_COUNT_TRAILING_ZEROS(call)        \
  SPEED_ROUTINE_COUNT_ZEROS_C (fun (c, n), 0, 0)


#define SPEED_ROUTINE_INVERT_LIMB_CALL(call)                    \
  {                                                             \
    unsigned   i, j;                                            \
    mp_limb_t  d, dinv=0;                                       \
    mp_ptr     xp = s->xp_block - 1;                            \
                                                                \
    s->time_divisor = SPEED_BLOCK_SIZE;                         \
                                                                \
    speed_starttime ();                                         \
    i = s->reps;                                                \
    do                                                          \
      {                                                         \
        j = SPEED_BLOCK_SIZE;                                   \
        do                                                      \
          {                                                     \
            d = dinv ^ xp[j];                                   \
            d |= GMP_LIMB_HIGHBIT;                             \
            do { call; } while (0);                             \
          }                                                     \
        while (--j != 0);                                       \
      }                                                         \
    while (--i != 0);                                           \
                                                                \
    /* don't let the compiler optimize everything away */       \
    noop_1 (dinv);                                              \
                                                                \
    return speed_endtime();                                     \
  }


#endif


#define SPEED_ROUTINE_MPN_BACK_TO_BACK(function)        \
  {                                                     \
    unsigned  i;                                        \
    speed_starttime ();                                 \
    i = s->reps;                                        \
    do                                                  \
      function ();                                      \
    while (--i != 0);                                   \
    return speed_endtime ();                            \
  }


#define SPEED_ROUTINE_MPN_ZERO_CALL(call)               \
  {                                                     \
    mp_ptr    wp;                                       \
    unsigned  i;                                        \
    double    t;                                        \
    TMP_DECL (marker);                                  \
                                                        \
    SPEED_RESTRICT_COND (s->size >= 0);                 \
                                                        \
    TMP_MARK (marker);                                  \
    wp = SPEED_TMP_ALLOC_LIMBS (s->size, s->align_wp);  \
    speed_operand_dst (s, wp, s->size);                 \
    speed_cache_fill (s);                               \
                                                        \
    speed_starttime ();                                 \
    i = s->reps;                                        \
    do                                                  \
      call;                                             \
    while (--i != 0);                                   \
    t = speed_endtime ();                               \
                                                        \
    TMP_FREE (marker);                                  \
    return t;                                           \
  }  

#define SPEED_ROUTINE_MPN_ZERO(function)                \
  SPEED_ROUTINE_MPN_ZERO_CALL (function (wp, s->size))
