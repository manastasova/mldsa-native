// Copyright (c) 2025 The mldsa-native project authors
// SPDX-License-Identifier: Apache-2.0

#include "fips202/fips202.h"

extern unsigned int keccak_absorb(uint64_t s[MLD_KECCAK_LANES], unsigned int pos, unsigned int r, const uint8_t *in, size_t inlen);

void harness(void)
{
  uint64_t *s;
  unsigned int pos;
  unsigned int r;
  const uint8_t *in;
  size_t inlen;

  keccak_absorb(s, pos, r, in, inlen);
}
