// Copyright (c) 2025 The mldsa-native project authors
// SPDX-License-Identifier: Apache-2.0

#include <stdint.h>
#include "../../../mldsa/cbmc.h"

#define NUM_ELEMS 5

// Preconditions - MAGIC WORD - requires()
// Memrory Regions - MAGIC WORD - assings()
// Post-Condition - MAGIC WORD - ensures()


uint32_t demo_add(uint32_t a, uint32_t b)
__contract__
(
  requires((uint64_t) a + b <= UINT32_MAX)
  ensures(return_value == a + b)
);















// Preconditions - MAGIC WORD - requires()
// Memrory Regions - MAGIC WORD - assings()
// Post-Condition - MAGIC WORD - ensures()




// uint32_t demo_add(uint32_t a, uint32_t b)
// __contract__(
//   requires(a <= UINT32_MAX - b)
//   ensures(return_value == a + b)
// );

// void demo_add_arrays(uint32_t *a, uint32_t *b, uint32_t *result, int elems)
// __contract__(
//   // PreConditions
//   requires(memory_no_alias(a, sizeof(uint32_t) * elems))
//   requires(memory_no_alias(b, sizeof(uint32_t) * elems))
//   requires(memory_no_alias(result, sizeof(uint32_t) *elems))
//   requires(forall(k, 0, elems, a[k] <= UINT32_MAX - b[k]))
//   assigns(memory_slice(result, sizeof(uint32_t) * elems))
//   // Post-Conditions
//  ensures(forall(k, 0, elems, result[k] == a[k] + b[k]))
// );




// uint32_t demo_add(uint32_t a, uint32_t b)
// __contract__(
//   requires(a <= UINT32_MAX - b)
//   ensures(return_value == a + b)
// );


// void demo_add_arrays(uint32_t *a, uint32_t *b, uint32_t *result, int NUM_ELEMS)
// __contract__(
//   // PreConditions
//   requires(memory_no_alias(a, sizeof(uint32_t) * NUM_ELEMS))
//   requires(memory_no_alias(b, sizeof(uint32_t) * NUM_ELEMS))
//   requires(memory_no_alias(result, sizeof(uint32_t) * NUM_ELEMS))
//   requires(forall(k, 0, NUM_ELEMS, a[k] <= UINT32_MAX - b[k]))
//   assigns(memory_slice(result, sizeof(uint32_t) * NUM_ELEMS))
//   // Post-Conditions
//   ensures(forall(i, 0, NUM_ELEMS, result[i] == a[i] + b[i]))
// );
