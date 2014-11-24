// Copyright (c) 2014 Fabian Barkhau <fabian.barkhau@gmail.com> 
// License: MIT (see LICENSE.TXT file)  

#ifndef GBX_MATH_H
#define GBX_MATH_H

#include <src/gbx/common.h>

#define PI            3.14159265358979323846

#define SQUARED(n)    ((n) * (n))

inline Sint32 relative_align(Sint32 r_num, Sint32 r_min, Sint32 r_max,
                             Sint32 n_min, Sint32 n_max){
  // normalize
  Sint32 r_len = r_max - r_min;
  Sint32 n_len = n_max - n_min;
  Sint32 r_pos = r_num - r_min;

  // calculate relative position
  Sint32 n_pos = 0;
  if (r_len != 0){ // avoid division by zero
    n_pos = (n_len * r_pos) / r_len;
  } 

  // denormalize
  return n_pos + n_min;
}

inline Uint32 isqrt(Uint32 n){
  return Sqrt(n);
}

#endif // GBX_MATH_H
