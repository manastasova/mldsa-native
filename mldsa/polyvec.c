/*
 * Copyright (c) 2025 The mldsa-native project authors
 * SPDX-License-Identifier: Apache-2.0
 */
#include "polyvec.h"
#include <stdint.h>
#include "params.h"
#include "poly.h"

/*************************************************
 * Name:        expand_mat
 *
 * Description: Implementation of ExpandA. Generates matrix A with uniformly
 *              random coefficients a_{i,j} by performing rejection
 *              sampling on the output stream of SHAKE128(rho|j|i)
 *
 * Arguments:   - polyvecl mat[MLDSA_K]: output matrix
 *              - const uint8_t rho[]: byte array containing seed rho
 **************************************************/
void polyvec_matrix_expand(polyvecl mat[MLDSA_K],
                           const uint8_t rho[MLDSA_SEEDBYTES])
{
  unsigned int i, j;

  for (i = 0; i < MLDSA_K; ++i)
  {
    for (j = 0; j < MLDSA_L; ++j)
    {
      poly_uniform(&mat[i].vec[j], rho, (i << 8) + j);
    }
  }
}

void polyvec_matrix_pointwise_montgomery(polyveck *t,
                                         const polyvecl mat[MLDSA_K],
                                         const polyvecl *v)
{
  unsigned int i;

  for (i = 0; i < MLDSA_K; ++i)
  {
    polyvecl_pointwise_acc_montgomery(&t->vec[i], &mat[i], v);
  }
}

/**************************************************************/
/************ Vectors of polynomials of length MLDSA_L **************/
/**************************************************************/

void polyvecl_uniform_eta(polyvecl *v, const uint8_t seed[MLDSA_CRHBYTES],
                          uint16_t nonce)
{
  unsigned int i;

  for (i = 0; i < MLDSA_L; ++i)
  {
    poly_uniform_eta(&v->vec[i], seed, nonce++);
  }
}

void polyvecl_uniform_gamma1(polyvecl *v, const uint8_t seed[MLDSA_CRHBYTES],
                             uint16_t nonce)
{
  unsigned int i;

  for (i = 0; i < MLDSA_L; ++i)
  {
    poly_uniform_gamma1(&v->vec[i], seed, MLDSA_L * nonce + i);
  }
}

void polyvecl_reduce(polyvecl *v)
{
  unsigned int i;

  for (i = 0; i < MLDSA_L; ++i)
  {
    poly_reduce(&v->vec[i]);
  }
}

/*************************************************
 * Name:        polyvecl_add
 *
 * Description: Add vectors of polynomials of length MLDSA_L.
 *              No modular reduction is performed.
 *
 * Arguments:   - polyvecl *w: pointer to output vector
 *              - const polyvecl *u: pointer to first summand
 *              - const polyvecl *v: pointer to second summand
 **************************************************/
void polyvecl_add(polyvecl *w, const polyvecl *u, const polyvecl *v)
{
  unsigned int i;

  for (i = 0; i < MLDSA_L; ++i)
  {
    poly_add(&w->vec[i], &u->vec[i], &v->vec[i]);
  }
}

/*************************************************
 * Name:        polyvecl_ntt
 *
 * Description: Forward NTT of all polynomials in vector of length MLDSA_L.
 *Output coefficients can be up to 16*MLDSA_Q larger than input coefficients.
 *
 * Arguments:   - polyvecl *v: pointer to input/output vector
 **************************************************/
void polyvecl_ntt(polyvecl *v)
{
  unsigned int i;

  for (i = 0; i < MLDSA_L; ++i)
  {
    poly_ntt(&v->vec[i]);
  }
}

void polyvecl_invntt_tomont(polyvecl *v)
{
  unsigned int i;

  for (i = 0; i < MLDSA_L; ++i)
  {
    poly_invntt_tomont(&v->vec[i]);
  }
}

void polyvecl_pointwise_poly_montgomery(polyvecl *r, const poly *a,
                                        const polyvecl *v)
{
  unsigned int i;

  for (i = 0; i < MLDSA_L; ++i)
  {
    poly_pointwise_montgomery(&r->vec[i], a, &v->vec[i]);
  }
}

/*************************************************
 * Name:        polyvecl_pointwise_acc_montgomery
 *
 * Description: Pointwise multiply vectors of polynomials of length MLDSA_L,
 *multiply resulting vector by 2^{-32} and add (accumulate) polynomials in it.
 *Input/output vectors are in NTT domain representation.
 *
 * Arguments:   - poly *w: output polynomial
 *              - const polyvecl *u: pointer to first input vector
 *              - const polyvecl *v: pointer to second input vector
 **************************************************/
void polyvecl_pointwise_acc_montgomery(poly *w, const polyvecl *u,
                                       const polyvecl *v)
{
  unsigned int i;
  poly t;

  poly_pointwise_montgomery(w, &u->vec[0], &v->vec[0]);
  for (i = 1; i < MLDSA_L; ++i)
  {
    poly_pointwise_montgomery(&t, &u->vec[i], &v->vec[i]);
    poly_add(w, w, &t);
  }
}

/*************************************************
 * Name:        polyvecl_chknorm
 *
 * Description: Check infinity norm of polynomials in vector of length MLDSA_L.
 *              Assumes input polyvecl to be reduced by polyvecl_reduce().
 *
 * Arguments:   - const polyvecl *v: pointer to vector
 *              - int32_t B: norm bound
 *
 * Returns 0 if norm of all polynomials is strictly smaller than B <=
 *(MLDSA_Q-1)/8 and 1 otherwise.
 **************************************************/
int polyvecl_chknorm(const polyvecl *v, int32_t bound)
{
  unsigned int i;

  for (i = 0; i < MLDSA_L; ++i)
  {
    if (poly_chknorm(&v->vec[i], bound))
    {
      return 1;
    }
  }

  return 0;
}

/**************************************************************/
/************ Vectors of polynomials of length MLDSA_K **************/
/**************************************************************/

void polyveck_uniform_eta(polyveck *v, const uint8_t seed[MLDSA_CRHBYTES],
                          uint16_t nonce)
{
  unsigned int i;

  for (i = 0; i < MLDSA_K; ++i)
  {
    poly_uniform_eta(&v->vec[i], seed, nonce++);
  }
}

/*************************************************
 * Name:        polyveck_reduce
 *
 * Description: Reduce coefficients of polynomials in vector of length MLDSA_K
 *              to representatives in [-6283008,6283008].
 *
 * Arguments:   - polyveck *v: pointer to input/output vector
 **************************************************/
void polyveck_reduce(polyveck *v)
{
  unsigned int i;

  for (i = 0; i < MLDSA_K; ++i)
  {
    poly_reduce(&v->vec[i]);
  }
}

/*************************************************
 * Name:        polyveck_caddq
 *
 * Description: For all coefficients of polynomials in vector of length MLDSA_K
 *              add MLDSA_Q if coefficient is negative.
 *
 * Arguments:   - polyveck *v: pointer to input/output vector
 **************************************************/
void polyveck_caddq(polyveck *v)
{
  unsigned int i;

  for (i = 0; i < MLDSA_K; ++i)
  {
    poly_caddq(&v->vec[i]);
  }
}

/*************************************************
 * Name:        polyveck_add
 *
 * Description: Add vectors of polynomials of length MLDSA_K.
 *              No modular reduction is performed.
 *
 * Arguments:   - polyveck *w: pointer to output vector
 *              - const polyveck *u: pointer to first summand
 *              - const polyveck *v: pointer to second summand
 **************************************************/
void polyveck_add(polyveck *w, const polyveck *u, const polyveck *v)
{
  unsigned int i;

  for (i = 0; i < MLDSA_K; ++i)
  {
    poly_add(&w->vec[i], &u->vec[i], &v->vec[i]);
  }
}

/*************************************************
 * Name:        polyveck_sub
 *
 * Description: Subtract vectors of polynomials of length MLDSA_K.
 *              No modular reduction is performed.
 *
 * Arguments:   - polyveck *w: pointer to output vector
 *              - const polyveck *u: pointer to first input vector
 *              - const polyveck *v: pointer to second input vector to be
 *                                   subtracted from first input vector
 **************************************************/
void polyveck_sub(polyveck *w, const polyveck *u, const polyveck *v)
{
  unsigned int i;

  for (i = 0; i < MLDSA_K; ++i)
  {
    poly_sub(&w->vec[i], &u->vec[i], &v->vec[i]);
  }
}

/*************************************************
 * Name:        polyveck_shiftl
 *
 * Description: Multiply vector of polynomials of Length MLDSA_K by 2^MLDSA_D
 *without modular reduction. Assumes input coefficients to be less than
 *2^{31-MLDSA_D}.
 *
 * Arguments:   - polyveck *v: pointer to input/output vector
 **************************************************/
void polyveck_shiftl(polyveck *v)
{
  unsigned int i;

  for (i = 0; i < MLDSA_K; ++i)
  {
    poly_shiftl(&v->vec[i]);
  }
}

/*************************************************
 * Name:        polyveck_ntt
 *
 * Description: Forward NTT of all polynomials in vector of length MLDSA_K.
 *Output coefficients can be up to 16*MLDSA_Q larger than input coefficients.
 *
 * Arguments:   - polyveck *v: pointer to input/output vector
 **************************************************/
void polyveck_ntt(polyveck *v)
{
  unsigned int i;

  for (i = 0; i < MLDSA_K; ++i)
  {
    poly_ntt(&v->vec[i]);
  }
}

/*************************************************
 * Name:        polyveck_invntt_tomont
 *
 * Description: Inverse NTT and multiplication by 2^{32} of polynomials
 *              in vector of length MLDSA_K. Input coefficients need to be less
 *              than 2*MLDSA_Q.
 *
 * Arguments:   - polyveck *v: pointer to input/output vector
 **************************************************/
void polyveck_invntt_tomont(polyveck *v)
{
  unsigned int i;

  for (i = 0; i < MLDSA_K; ++i)
  {
    poly_invntt_tomont(&v->vec[i]);
  }
}

void polyveck_pointwise_poly_montgomery(polyveck *r, const poly *a,
                                        const polyveck *v)
{
  unsigned int i;

  for (i = 0; i < MLDSA_K; ++i)
  {
    poly_pointwise_montgomery(&r->vec[i], a, &v->vec[i]);
  }
}


/*************************************************
 * Name:        polyveck_chknorm
 *
 * Description: Check infinity norm of polynomials in vector of length MLDSA_K.
 *              Assumes input polyveck to be reduced by polyveck_reduce().
 *
 * Arguments:   - const polyveck *v: pointer to vector
 *              - int32_t B: norm bound
 *
 * Returns 0 if norm of all polynomials are strictly smaller than B <=
 *(MLDSA_Q-1)/8 and 1 otherwise.
 **************************************************/
int polyveck_chknorm(const polyveck *v, int32_t bound)
{
  unsigned int i;

  for (i = 0; i < MLDSA_K; ++i)
  {
    if (poly_chknorm(&v->vec[i], bound))
    {
      return 1;
    }
  }

  return 0;
}

/*************************************************
 * Name:        polyveck_power2round
 *
 * Description: For all coefficients a of polynomials in vector of length
 *MLDSA_K, compute a0, a1 such that a mod^+ MLDSA_Q = a1*2^MLDSA_D + a0 with
 *-2^{MLDSA_D-1} < a0 <= 2^{MLDSA_D-1}. Assumes coefficients to be standard
 *representatives.
 *
 * Arguments:   - polyveck *v1: pointer to output vector of polynomials with
 *                              coefficients a1
 *              - polyveck *v0: pointer to output vector of polynomials with
 *                              coefficients a0
 *              - const polyveck *v: pointer to input vector
 **************************************************/
void polyveck_power2round(polyveck *v1, polyveck *v0, const polyveck *v)
{
  unsigned int i;

  for (i = 0; i < MLDSA_K; ++i)
  {
    poly_power2round(&v1->vec[i], &v0->vec[i], &v->vec[i]);
  }
}

/*************************************************
 * Name:        polyveck_decompose
 *
 * Description: For all coefficients a of polynomials in vector of length
 *MLDSA_K, compute high and low bits a0, a1 such a mod^+ MLDSA_Q = a1*ALPHA
 *+ a0 with -ALPHA/2 < a0 <= ALPHA/2 except a1 = (MLDSA_Q-1)/ALPHA where we set
 *a1 = 0 and -ALPHA/2 <= a0 = a mod MLDSA_Q - MLDSA_Q < 0. Assumes coefficients
 *to be standard representatives.
 *
 * Arguments:   - polyveck *v1: pointer to output vector of polynomials with
 *                              coefficients a1
 *              - polyveck *v0: pointer to output vector of polynomials with
 *                              coefficients a0
 *              - const polyveck *v: pointer to input vector
 **************************************************/
void polyveck_decompose(polyveck *v1, polyveck *v0, const polyveck *v)
{
  unsigned int i;

  for (i = 0; i < MLDSA_K; ++i)
  {
    poly_decompose(&v1->vec[i], &v0->vec[i], &v->vec[i]);
  }
}

/*************************************************
 * Name:        polyveck_make_hint
 *
 * Description: Compute hint vector.
 *
 * Arguments:   - polyveck *h: pointer to output vector
 *              - const polyveck *v0: pointer to low part of input vector
 *              - const polyveck *v1: pointer to high part of input vector
 *
 * Returns number of 1 bits.
 **************************************************/
unsigned int polyveck_make_hint(polyveck *h, const polyveck *v0,
                                const polyveck *v1)
{
  unsigned int i, s = 0;

  for (i = 0; i < MLDSA_K; ++i)
  __loop__(
    assigns(i, s, object_whole(h))
    invariant(i <= MLDSA_K)
    invariant(s <= i * MLDSA_N)
  )
  {
    s += poly_make_hint(&h->vec[i], &v0->vec[i], &v1->vec[i]);
  }

  return s;
}

/*************************************************
 * Name:        polyveck_use_hint
 *
 * Description: Use hint vector to correct the high bits of input vector.
 *
 * Arguments:   - polyveck *w: pointer to output vector of polynomials with
 *                             corrected high bits
 *              - const polyveck *u: pointer to input vector
 *              - const polyveck *h: pointer to input hint vector
 **************************************************/
void polyveck_use_hint(polyveck *w, const polyveck *u, const polyveck *h)
{
  unsigned int i;

  for (i = 0; i < MLDSA_K; ++i)
  {
    poly_use_hint(&w->vec[i], &u->vec[i], &h->vec[i]);
  }
}

void polyveck_pack_w1(uint8_t r[MLDSA_K * MLDSA_POLYW1_PACKEDBYTES],
                      const polyveck *w1)
{
  unsigned int i;

  for (i = 0; i < MLDSA_K; ++i)
  {
    polyw1_pack(&r[i * MLDSA_POLYW1_PACKEDBYTES], &w1->vec[i]);
  }
}

void polyveck_pack_eta(uint8_t r[MLDSA_K * MLDSA_POLYETA_PACKEDBYTES],
                       const polyveck *p)
{
  unsigned int i;
  for (i = 0; i < MLDSA_K; ++i)
  {
    polyeta_pack(r + i * MLDSA_POLYETA_PACKEDBYTES, &p->vec[i]);
  }
}

void polyvecl_pack_eta(uint8_t r[MLDSA_L * MLDSA_POLYETA_PACKEDBYTES],
                       const polyvecl *p)
{
  unsigned int i;
  for (i = 0; i < MLDSA_L; ++i)
  {
    polyeta_pack(r + i * MLDSA_POLYETA_PACKEDBYTES, &p->vec[i]);
  }
}

void polyvecl_pack_z(uint8_t r[MLDSA_L * MLDSA_POLYZ_PACKEDBYTES],
                     const polyvecl *p)
{
  unsigned int i;
  for (i = 0; i < MLDSA_L; ++i)
  {
    polyz_pack(r + i * MLDSA_POLYZ_PACKEDBYTES, &p->vec[i]);
  }
}


void polyveck_pack_t0(uint8_t r[MLDSA_K * MLDSA_POLYT0_PACKEDBYTES],
                      const polyveck *p)
{
  unsigned int i;
  for (i = 0; i < MLDSA_K; ++i)
  {
    polyt0_pack(r + i * MLDSA_POLYT0_PACKEDBYTES, &p->vec[i]);
  }
}

void polyvecl_unpack_eta(polyvecl *p,
                         const uint8_t r[MLDSA_L * MLDSA_POLYETA_PACKEDBYTES])
{
  unsigned int i;
  for (i = 0; i < MLDSA_L; ++i)
  {
    polyeta_unpack(&p->vec[i], r + i * MLDSA_POLYETA_PACKEDBYTES);
  }
}

void polyveck_unpack_eta(polyveck *p,
                         const uint8_t r[MLDSA_K * MLDSA_POLYETA_PACKEDBYTES])
{
  unsigned int i;
  for (i = 0; i < MLDSA_K; ++i)
  {
    polyeta_unpack(&p->vec[i], r + i * MLDSA_POLYETA_PACKEDBYTES);
  }
}

void polyveck_unpack_t0(polyveck *p,
                        const uint8_t r[MLDSA_K * MLDSA_POLYT0_PACKEDBYTES])
{
  unsigned int i;
  for (i = 0; i < MLDSA_K; ++i)
  {
    polyt0_unpack(&p->vec[i], r + i * MLDSA_POLYT0_PACKEDBYTES);
  }
}
