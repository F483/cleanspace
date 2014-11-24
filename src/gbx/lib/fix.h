// Copyright (c) 2014 Fabian Barkhau <abian.barkhau@gmail.com>
// License: MIT (see LICENSE.TXT file)

#ifndef GBX_FIX_H
#define GBX_FIX_H

#include <src/gbx/common.h>
#include <src/gbx/lib/math.h>

// 100% pure inline functional code !

WRAP_DEF(Sint32, Fix);

#define FIX_SHIFT     8
#define FIX_SCALE     (1 << FIX_SHIFT)
#define FIX_SCALEF    ((float)FIX_SCALE)

#define FIX_ONE       WRAP_CONST(FIX_SCALE)
#define fix_one       WRAP(Fix, FIX_SCALE)
#define FIX_ZERO      WRAP_CONST(0)
#define fix_zero      WRAP(Fix, 0)

// FIXME understand and fix it
//! Get the fixed point reciprocal of \a a, in \a fp fractional bits.
//!
//  \param a  Value to take the reciprocal of.
//  \param fp   Number of fixed point bits
//  \note   The routine does do a division, but the compiler will
//  optimize it to a single constant ... \e if both \a a and \a fp
//  are constants!
//  \sa #FX_RECIMUL
//
#define FX_RECIPROCAL(a, fp)  ( ((1<<(fp))+(a)-1)/(a) )

// FIXME understand and fix it
//! Perform the division \a x/ \a a by reciprocal multiplication
//! Division is slow, but you can approximate division by a constant
//  by multiplying with its reciprocal: x/a vs x*(1/a). This routine
//  gives the reciprocal of \a a as a fixed point number with \a fp
//  fractional bits.
//  \param a  Value to take the reciprocal of.
//  \param fp   Number of fixed point bits
//  \note   The routine does do a division, but the compiler will
//  optimize it to a single constant ... \e if both \a a and \a fp
//  are constants!
//  \note   Rules for safe reciprocal division, using
//  n = 2<sup>fp</sup> and m = (n+a-1)/a (i.e., rounding up)
//  \li   Maximum safe numerator \a x:  x < n/(m*a-n)
//  \li   Minimum n for known \a x:     n > x*(a-1)
//
#define FX_RECIMUL(x, a, fp)  ( ((x)*((1<<(fp))+(a)-1)/(a))>>(fp) )

inline Fix int_to_fix(Sint32 d){
  return WRAP(Fix, d << FIX_SHIFT);
}

inline Sint32 fix_to_int(Fix fix){
  return WRAP_GET(fix) / FIX_SCALE;
}

inline Fix float_to_fix(float f){
  return WRAP(Fix, f * FIX_SCALEF);
}

inline float fix_to_float(Fix fix){
  return WRAP_GET(fix) / FIX_SCALEF;
}

inline Bool fix_eq(Fix a, Fix b){
  return WRAP_GET(a) == WRAP_GET(b);
}

inline Bool fix_neq(Fix a, Fix b){
  return !fix_eq(a, b);
}

inline Fix fix_add(Fix a, Fix b){
  return WRAP(Fix, WRAP_GET(a) + WRAP_GET(b));
}

inline Fix fix_sub(Fix a, Fix b){
  return WRAP(Fix, WRAP_GET(a) - WRAP_GET(b));
}

inline Fix fix_mul(Fix a, Fix b){
  return WRAP(Fix, (WRAP_GET(a) * WRAP_GET(b)) >> FIX_SHIFT);
}

inline Fix fix_div(Fix a, Fix b){
  return WRAP(Fix, (WRAP_GET(a) * FIX_SCALE) / WRAP_GET(b));
}

// use 64bit math
inline Fix fix_mul64(Fix a, Fix b){
  return WRAP(Fix, (((Sint64)WRAP_GET(a)) * WRAP_GET(b)) >> FIX_SHIFT);
}

// use 64bit math
inline Fix fix_div64(Fix a, Fix b){
  return WRAP(Fix, (((Sint64)WRAP_GET(a)) << FIX_SHIFT) / WRAP_GET(b));
}

inline Fix fix_abs(Fix fix){
  return WRAP(Fix, abs(WRAP_GET(fix)));
}

inline Fix fix_sqrt(Fix fix){ // FIXME this is bullshit
  Uint32 i = fix_to_int(fix);
  if (i != 0){
    return int_to_fix(isqrt(i));
  }
  return fix_zero;
}

inline Bool fix_nonzero(Fix fix){
  return !fix_eq(fix, fix_zero);
}

inline Fix fix_neg(Fix fix){
  return fix_mul(fix, int_to_fix(-1));
}

inline Bool fix_gt(Fix l, Fix r){ // TODO fast implementation
  return fix_to_float(l) > fix_to_float(r);
}

inline Bool fix_gte(Fix l, Fix r){ // TODO fast implementation
  return fix_to_float(l) >= fix_to_float(r);
}

inline Bool fix_lt(Fix l, Fix r){ // TODO fast implementation
  return fix_to_float(l) < fix_to_float(r);
}

inline Bool fix_lte(Fix l, Fix r){ // TODO fast implementation
  return fix_to_float(l) <= fix_to_float(r);
}

inline Fix fix_deg_to_rad(Fix deg){
  return fix_mul64(deg, fix_div64(float_to_fix(PI), float_to_fix(180.0)));
}

inline Fix fix_rad_to_deg(Fix rad){
  return fix_mul64(rad, fix_div64(float_to_fix(180.0), float_to_fix(PI)));
}

inline Fix fix_cos(Fix fix){
  return float_to_fix(cos(fix_to_float(fix)));
}

inline Fix fix_sin(Fix fix){
  return float_to_fix(sin(fix_to_float(fix)));
}

inline Fix fix_atan2(Fix y, Fix x){
  return float_to_fix(atan2(fix_to_float(y), fix_to_float(x)));
}

#endif
