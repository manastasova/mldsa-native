// Copyright (c) 2025 The mldsa-native project authors
// SPDX-License-Identifier: Apache-2.0

#include "fips202/fips202.h"

extern void store64(uint8_t x[8], const uint64_t u);

void harness(void)
{
  uint8_t *x;
  const uint64_t u;
  store64(x, u);
}
