//===-- floatundidf.c - Implement __floatundidf ---------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements __floatundidf for the compiler_rt library.
//
//===----------------------------------------------------------------------===//

// Returns: convert a to a double, rounding toward even.

// Assumption: double is a IEEE 64 bit floating point type
//             du_int is a 64 bit integral type

// seee eeee eeee mmmm mmmm mmmm mmmm mmmm | mmmm mmmm mmmm mmmm mmmm mmmm mmmm
// mmmm

#include "int_lib.h"

double __attribute__ ((noinline)) __floatundidf_(du_int a) {
  if (a == 0)
    return 0.0;
  const unsigned N = sizeof(du_int) * CHAR_BIT;
  int sd = N - __builtin_clzll(a); // number of significant digits
  int e = sd - 1;                  // exponent
  if (sd > DBL_MANT_DIG) {
    //  start:  0000000000000000000001xxxxxxxxxxxxxxxxxxxxxxPQxxxxxxxxxxxxxxxxxx
    //  finish: 000000000000000000000000000000000000001xxxxxxxxxxxxxxxxxxxxxxPQR
    //                                                12345678901234567890123456
    //  1 = msb 1 bit
    //  P = bit DBL_MANT_DIG-1 bits to the right of 1
    //  Q = bit DBL_MANT_DIG bits to the right of 1
    //  R = "or" of all bits to the right of Q
    switch (sd) {
    case DBL_MANT_DIG + 1:
      a <<= 1;
      break;
    case DBL_MANT_DIG + 2:
      break;
    default:
      a = (a >> (sd - (DBL_MANT_DIG + 2))) |
          ((a & ((du_int)(-1) >> ((N + DBL_MANT_DIG + 2) - sd))) != 0);
    };
    // finish:
    a |= (a & 4) != 0; // Or P into R
    ++a;               // round - this step may add a significant bit
    a >>= 2;           // dump Q and R
    // a is now rounded to DBL_MANT_DIG or DBL_MANT_DIG+1 bits
    if (a & ((du_int)1 << DBL_MANT_DIG)) {
      a >>= 1;
      ++e;
    }
    // a is now rounded to DBL_MANT_DIG bits
  } else {
    a <<= (DBL_MANT_DIG - sd);
    // a is now rounded to DBL_MANT_DIG bits
  }
  double_bits fb;
  fb.u.s.high = ((su_int)(e + 1023) << 20) |      // exponent
                ((su_int)(a >> 32) & 0x000FFFFF); // mantissa-high
  fb.u.s.low = (su_int)a;                         // mantissa-low

  return fb.f;
}

COMPILER_RT_ABI double __floatundidf(du_int a) {
  return __floatundidf_(a);
}
