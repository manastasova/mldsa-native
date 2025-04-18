/*
 * Copyright (c) 2025 The mldsa-native project authors
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef MLD_SIGN_H
#define MLD_SIGN_H

#include <stddef.h>
#include <stdint.h>
#include "common.h"
#include "poly.h"
#include "polyvec.h"

#define crypto_sign_keypair_internal MLD_NAMESPACE(keypair_internal)
/*************************************************
 * Name:        crypto_sign_keypair_internal
 *
 * Description: FIPS 204: Algorithm 6 ML-DSA.KeyGen_internal.
 *              Generates public and private key. Internal API.
 *
 * Arguments:   - uint8_t *pk:   pointer to output public key (allocated
 *                               array of CRYPTO_PUBLICKEYBYTES bytes)
 *              - uint8_t *sk:   pointer to output private key (allocated
 *                               array of CRYPTO_SECRETKEYBYTES bytes)
 *              - uint8_t *seed: pointer to input random seed (MLDSA_SEEDBYTES
 *bytes)
 *
 * Returns 0 (success)
 **************************************************/
int crypto_sign_keypair_internal(uint8_t *pk, uint8_t *sk,
                                 const uint8_t seed[MLDSA_SEEDBYTES]);

#define crypto_sign_keypair MLD_NAMESPACE(keypair)
/*************************************************
 * Name:        crypto_sign_keypair
 *
 * Description: FIPS 204: Algorithm 1 ML-DSA.KeyGen
 *              Generates public and private key.
 *
 * Arguments:   - uint8_t *pk:   pointer to output public key (allocated
 *                               array of CRYPTO_PUBLICKEYBYTES bytes)
 *              - uint8_t *sk:   pointer to output private key (allocated
 *                               array of CRYPTO_SECRETKEYBYTES bytes)
 *
 * Returns 0 (success)
 **************************************************/
int crypto_sign_keypair(uint8_t *pk, uint8_t *sk);

#define crypto_sign_signature_internal MLD_NAMESPACE(signature_internal)
/*************************************************
 * Name:        crypto_sign_signature_internal
 *
 * Description: Computes signature. Internal API.
 *
 * Arguments:   - uint8_t *sig:   pointer to output signature (of length
 *CRYPTO_BYTES)
 *              - size_t *siglen: pointer to output length of signature
 *              - uint8_t *m:     pointer to message to be signed
 *              - size_t mlen:    length of message
 *              - uint8_t *pre:   pointer to prefix string
 *              - size_t prelen:  length of prefix string
 *              - uint8_t *rnd:   pointer to random seed
 *              - uint8_t *sk:    pointer to bit-packed secret key
 *              - int externalmu: indicates input message m is processed as mu
 *
 * Returns 0 (success)
 **************************************************/
int crypto_sign_signature_internal(uint8_t *sig, size_t *siglen,
                                   const uint8_t *m, size_t mlen,
                                   const uint8_t *pre, size_t prelen,
                                   const uint8_t rnd[MLDSA_RNDBYTES],
                                   const uint8_t *sk, int externalmu);

#define crypto_sign_signature MLD_NAMESPACE(signature)
/*************************************************
 * Name:        crypto_sign_signature
 *
 * Description: FIPS 204: Algorithm 2 ML-DSA.Sign.
 *              Computes signature.
 *
 * Arguments:   - uint8_t *sig:   pointer to output signature (of length
 *CRYPTO_BYTES)
 *              - size_t *siglen: pointer to output length of signature
 *              - uint8_t *m:     pointer to message to be signed
 *              - size_t mlen:    length of message
 *              - uint8_t *ctx:   pointer to contex string
 *              - size_t ctxlen:  length of contex string
 *              - uint8_t *sk:    pointer to bit-packed secret key
 *
 * Returns 0 (success) or -1 (context string too long)
 **************************************************/
int crypto_sign_signature(uint8_t *sig, size_t *siglen, const uint8_t *m,
                          size_t mlen, const uint8_t *ctx, size_t ctxlen,
                          const uint8_t *sk);

#define crypto_sign_signature_extmu MLD_NAMESPACE(signature_extmu)
/*************************************************
 * Name:        crypto_sign_signature_extmu
 *
 * Description:  FIPS 204: Algorithm 2 ML-DSA.Sign external mu variant.
 *               Computes signature.
 *
 * Arguments:   - uint8_t *sig:   pointer to output signature (of length
 *CRYPTO_BYTES)
 *              - size_t *siglen: pointer to output length of signature
 *              - uint8_t mu:     input mu to be signed of size MLDSA_CRHBYTES
 *              - uint8_t *sk:    pointer to bit-packed secret key
 *
 * Returns 0 (success) or -1 (context string too long)
 **************************************************/
int crypto_sign_signature_extmu(uint8_t *sig, size_t *siglen,
                                const uint8_t mu[MLDSA_CRHBYTES],
                                const uint8_t *sk);

#define crypto_sign MLD_NAMESPACETOP
/*************************************************
 * Name:        crypto_sign
 *
 * Description: Compute signed message.
 *
 * Arguments:   - uint8_t *sm: pointer to output signed message (allocated
 *                             array with CRYPTO_BYTES + mlen bytes),
 *                             can be equal to m
 *              - size_t *smlen: pointer to output length of signed
 *                               message
 *              - const uint8_t *m: pointer to message to be signed
 *              - size_t mlen: length of message
 *              - const uint8_t *ctx: pointer to context string
 *              - size_t ctxlen: length of context string
 *              - const uint8_t *sk: pointer to bit-packed secret key
 *
 * Returns 0 (success) or -1 (context string too long)
 **************************************************/
int crypto_sign(uint8_t *sm, size_t *smlen, const uint8_t *m, size_t mlen,
                const uint8_t *ctx, size_t ctxlen, const uint8_t *sk);

#define crypto_sign_verify_internal MLD_NAMESPACE(verify_internal)
/*************************************************
 * Name:        crypto_sign_verify_internal
 *
 * Description: FIPS 204: Algorithm 8 ML-DSA.Verify_internal.
 *              Verifies signature. Internal API.
 * Arguments:   - uint8_t *m: pointer to input signature
 *              - size_t siglen: length of signature
 *              - const uint8_t *m: pointer to message
 *              - size_t mlen: length of message
 *              - const uint8_t *pre: pointer to prefix string
 *              - size_t prelen: length of prefix string
 *              - const uint8_t *pk: pointer to bit-packed public key
 *               - int externalmu: indicates input message m is processed as mu
 *
 * Returns 0 if signature could be verified correctly and -1 otherwise
 **************************************************/
int crypto_sign_verify_internal(const uint8_t *sig, size_t siglen,
                                const uint8_t *m, size_t mlen,
                                const uint8_t *pre, size_t prelen,
                                const uint8_t *pk, int externalmu);

#define crypto_sign_verify MLD_NAMESPACE(verify)
/*************************************************
 * Name:        crypto_sign_verify
 *
 * Description: FIPS 204: Algorithm 3 ML-DSA.Verify.
 *              Verifies signature.
 *
 * Arguments:   - uint8_t *m: pointer to input signature
 *              - size_t siglen: length of signature
 *              - const uint8_t *m: pointer to message
 *              - size_t mlen: length of message
 *              - const uint8_t *ctx: pointer to context string
 *              - size_t ctxlen: length of context string
 *              - const uint8_t *pk: pointer to bit-packed public key
 *
 * Returns 0 if signature could be verified correctly and -1 otherwise
 **************************************************/
int crypto_sign_verify(const uint8_t *sig, size_t siglen, const uint8_t *m,
                       size_t mlen, const uint8_t *ctx, size_t ctxlen,
                       const uint8_t *pk);

#define crypto_sign_verify_extmu MLD_NAMESPACE(verify_extmu)
/*************************************************
 * Name:        crypto_sign_verify_extmu
 *
 * Description: FIPS 204: Algorithm 3 ML-DSA.Verify external mu variant.
 *              Verifies signature.
 *
 * Arguments:   - uint8_t *m: pointer to input signature
 *              - size_t siglen: length of signature
 *              - const uint8_t mu: input mu of size MLDSA_CRHBYTES
 *              - const uint8_t *pk: pointer to bit-packed public key
 *
 * Returns 0 if signature could be verified correctly and -1 otherwise
 **************************************************/
int crypto_sign_verify_extmu(const uint8_t *sig, size_t siglen,
                             const uint8_t mu[MLDSA_CRHBYTES],
                             const uint8_t *pk);

#define crypto_sign_open MLD_NAMESPACE(open)
/*************************************************
 * Name:        crypto_sign_open
 *
 * Description: Verify signed message.
 *
 * Arguments:   - uint8_t *m: pointer to output message (allocated
 *                            array with smlen bytes), can be equal to sm
 *              - size_t *mlen: pointer to output length of message
 *              - const uint8_t *sm: pointer to signed message
 *              - size_t smlen: length of signed message
 *              - const uint8_t *ctx: pointer to context tring
 *              - size_t ctxlen: length of context string
 *              - const uint8_t *pk: pointer to bit-packed public key
 *
 * Returns 0 if signed message could be verified correctly and -1 otherwise
 **************************************************/
int crypto_sign_open(uint8_t *m, size_t *mlen, const uint8_t *sm, size_t smlen,
                     const uint8_t *ctx, size_t ctxlen, const uint8_t *pk);

#endif /* !MLD_SIGN_H */
