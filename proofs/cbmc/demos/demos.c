// Copyright (c) 2025 The mldsa-native project authors
// SPDX-License-Identifier: Apache-2.0

#include "demos.h"


uint32_t demo_add(uint32_t a, uint32_t b) {
  return a + b;
};

















// uint32_t demo_add(uint32_t a, uint32_t b) {

//   return a + b;
// }

// void demo_add_arrays(uint32_t *a, uint32_t *b, uint32_t *result, int elems) {

//     for (int i = 0; i < elems; i++)
//     __loop__(
//       invariant(0 <= i && i <= elems)
//       invariant(forall(k, 0, i, result[k] == a[k] + b[k]))
//     )
//     {
//       result[i] = demo_add(a[i], b[i]);
//     }
//   }








// uint32_t demo_add(uint32_t a, uint32_t b) {
//   uint32_t result;
//   result = a + b;

//   return result;
// }

// void demo_add_arrays(uint32_t *a, uint32_t *b, uint32_t *result, int NUM_ELEMS) {

//   for (int i = 0; i < NUM_ELEMS; i++)
//   __loop__(
//     invariant(0 <= i && i <= NUM_ELEMS)
//     invariant(forall(k, 0, i, result[k] == a[k] + b[k]))
//   )
//   {
//     result[i] = demo_add(a[i], b[i]);
//   }
// }
