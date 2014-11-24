// Copyright (c) 2014 Fabian Barkhau <fabian.barkhau@gmail.com> 
// License: MIT (see LICENSE.TXT file)  

#ifndef GBX_VEC_H
#define GBX_VEC_H

#include <src/gbx/common.h>
#include <src/gbx/lib/fix.h>

// 2D Vector
typedef struct { 
  Fix x, y; // Never access directly! Use vec_get_* and vec_set_*
} Vec;

#define VEC_ZERO        { FIX_ZERO, FIX_ZERO }
#define vec_zero        ((Vec){ fix_zero, fix_zero })

//////////////////
// CONSTRUCTORS //
//////////////////

inline Vec vec_make(Fix x, Fix y){
  return (Vec){ x, y };
}

inline Vec vec_make_int(Sint32 x, Sint32 y){ // for convience
  return vec_make(int_to_fix(x), int_to_fix(y));
}

inline Vec vec_make_float(float x, float y){ // for convience
  return vec_make(float_to_fix(x), float_to_fix(y));
}


/////////////
// GETTERS //
/////////////

inline Fix vec_get_x(Vec v){
  return v.x;
}

inline Fix vec_get_y(Vec v){
  return v.y;
}

inline Sint32 vec_get_x_int(Vec v){ // for convience
  return fix_to_int(vec_get_x(v));
}

inline Sint32 vec_get_y_int(Vec v){ // for convience
  return fix_to_int(vec_get_y(v));
}

inline float vec_get_x_float(Vec v){ // for convience
  return fix_to_float(vec_get_x(v));
}

inline float vec_get_y_float(Vec v){ // for convience
  return fix_to_float(vec_get_y(v));
}


/////////////
// SETTERS //
/////////////

inline Vec vec_set_x(Vec v, Fix x){
  v.x = x;
  return v;
}

inline Vec vec_set_y(Vec v, Fix y){
  v.y = y;
  return v;
}

inline Vec vec_set_x_int(Vec v, Sint32 x){ // for convience
  return vec_set_x(v, int_to_fix(x));
}

inline Vec vec_set_y_int(Vec v, Sint32 y){ // for convience
  return vec_set_y(v, int_to_fix(y));
}


//////////////
// EQUALITY //
//////////////

inline Bool vec_eq(Vec l, Vec r){
  return fix_eq(l.x, r.x) and fix_eq(l.y, r.y);
} 

inline Bool vec_neq(Vec l, Vec r){
  return !vec_eq(l, r);
} 

//////////////
// ADDITION //
//////////////

inline Vec vec_add(Vec l, Vec r){
  return vec_make(fix_add(l.x, r.x), fix_add(l.y, r.y)); 
} 

inline Vec vec_add_fix(Vec v, Fix f){ // for convience
  return vec_add(v, vec_make(f, f));
} 

inline Vec vec_add_int(Vec v, Sint32 i){ // for convience
  return vec_add(v, vec_make_int(i, i));
} 


/////////////////
// SUBTRACTION //
/////////////////

inline Vec vec_sub(Vec l, Vec r){
  return vec_make(fix_sub(l.x, r.x), fix_sub(l.y, r.y)); 
} 

inline Vec vec_sub_fix(Vec v, Fix f){ // for convience
  return vec_sub(v, vec_make(f, f));
} 

inline Vec vec_sub_int(Vec v, Sint32 i){ // for convience
  return vec_sub(v, vec_make_int(i, i));
} 


////////////////////
// MULTIPLICATION //
////////////////////

inline Vec vec_mul(Vec l, Vec r){
  return vec_make(fix_mul64(l.x, r.x), fix_mul64(l.y, r.y)); 
} 

inline Vec vec_mul_fix(Vec v, Fix f){ // for convience
  return vec_mul(v, vec_make(f, f));
} 

inline Vec vec_mul_int(Vec v, Sint32 i){ // for convience
  return vec_mul(v, vec_make_int(i, i));
} 


//////////////
// DIVISION //
//////////////

inline Vec vec_div(Vec l, Vec r){
  return vec_make(fix_div64(l.x, r.x), fix_div64(l.y, r.y)); 
} 

inline Vec vec_div_fix(Vec v, Fix f){ // for convience
  return vec_div(v, vec_make(f, f));
} 

inline Vec vec_div_int(Vec v, Sint32 i){ // for convience
  return vec_div(v, vec_make_int(i, i));
} 


//////////////
// NIGATION //
//////////////

inline Vec vec_neg(Vec v){
  return vec_mul_int(v, -1); 
}


////////////
// LENGTH //
////////////

inline Fix vec_length_sqrd(Vec v){
  return fix_add(fix_mul64(v.x, v.x), fix_mul64(v.y, v.y));
}

inline Fix vec_length(Vec v){
  return fix_sqrt(vec_length_sqrd(v));
}

inline Vec vec_set_length(Vec v, Fix value){
  Fix length = vec_length(v);
  if (fix_nonzero(length)){ // avoid division by zero
    Fix x = fix_div64(fix_mul64(v.x, value), length);
    Fix y = fix_div64(fix_mul64(v.y, value), length);
    return vec_make(x, y);
  }
  return vec_zero;
}

inline Uint32 vec_length_sqrd_int(Vec v){ // for convience
  return fix_to_int(vec_length_sqrd(v));
}

inline Uint32 vec_length_int(Vec v){ // for convience
  return fix_to_int(vec_length(v));
}

inline Vec vec_set_length_int(Vec v, Sint32 value){ // for convience
  return vec_set_length(v, int_to_fix(value));
}


//////////////
// DISTANCE //
//////////////

inline Fix vec_distance_sqrd(Vec l, Vec r){
  Vec delta = vec_sub(l, r);
  return fix_add(fix_mul64(delta.x, delta.x), fix_mul64(delta.y, delta.y));
}

inline Fix vec_distance(Vec l, Vec r){
  return fix_sqrt(vec_distance_sqrd(l, r));
}

inline Uint32 vec_distance_sqrd_int(Vec l, Vec r){ // for convience
  return fix_to_int(vec_distance_sqrd(l, r));
}

inline Uint32 vec_distance_int(Vec l, Vec r){ // for convience
  return fix_to_int(vec_distance(l, r));
}

inline Vec vec_normalize(Vec v){
  Fix length = vec_length(v);
  if(fix_nonzero(length)){
    return vec_div_fix(v, length);
  }
  return vec_zero;
}

inline Vec vec_perpendicular(Vec v){
  return vec_make(fix_neg(v.y), v.x);
}

inline Fix vec_dot(Vec l, Vec r){
  return fix_add(fix_mul64(l.x, r.x), fix_mul64(l.y, r.y));
}

inline Fix vec_cross(Vec l, Vec r){
  return fix_sub(fix_mul64(l.x, r.y), fix_mul64(l.y, r.x));
}
    
inline Vec vec_projection(Vec l, Vec r){
  return vec_mul_fix(r, fix_div64(vec_dot(l, r), vec_length_sqrd(r)));
}

inline Vec vec_interpolate_to(Vec l, Vec r, Fix range){
  Fix x = fix_add(l.x, fix_mul64(fix_sub(r.x, l.x), range));
  Fix y = fix_add(l.y, fix_mul64(fix_sub(r.y, l.y), range));
  return vec_make(x, y);
}

inline Vec vec_convert_to_basis(Vec v, Vec vx, Vec vy){
  Fix x = fix_div64(vec_dot(v, vx), vec_length_sqrd(vx));
  Fix y = fix_div64(vec_dot(v, vy), vec_length_sqrd(vy));
  return vec_make(x, y);
}

inline Fix vec_angle_rad(Vec v){
  if (fix_nonzero(vec_length_sqrd(v))){
    return fix_atan2(v.y, v.x);
  }
  return fix_zero;
}

inline Fix vec_angle_deg(Vec v){
  return fix_rad_to_deg(vec_angle_rad(v));
}

inline Vec vec_rotate_by_rad(Vec v, Fix radians){
  Fix c = fix_cos(radians);
  Fix s = fix_sin(radians);
  Fix x = fix_sub(fix_mul64(v.x, c), fix_mul64(v.y, s));
  Fix y = fix_add(fix_mul64(v.x, s), fix_mul64(v.y, c));
  return vec_make(x, y);
}

inline Vec vec_rotate_by_deg(Vec v, Fix degrees){
  return vec_rotate_by_rad(v, fix_deg_to_rad(degrees));
}

inline Vec vec_rotate_to_rad(Vec v, Fix radians){
  return vec_rotate_by_rad(vec_make(vec_length(v), fix_zero), radians);
}

inline Vec vec_rotate_to_deg(Vec v, Fix degrees){
  return vec_rotate_to_rad(v, fix_deg_to_rad(degrees));
}

inline Fix vec_angle_between_rad(Vec l, Vec r){
  Fix cross = vec_cross(l, r);
  Fix dot = vec_dot(l, r);
  return fix_atan2(cross, dot);
}

inline Fix vec_angle_between_deg(Vec l, Vec r){
  return fix_rad_to_deg(vec_angle_between_rad(l, r));
}

/**
 * Returns: Vec for "pos" so that "pos" reaches "dest" when "ttl" is 0.
 */
inline Vec vec_approach(Uint32 ttl, Vec pos, Vec dest){
  Vec curr_delta = vec_sub(pos, dest);
  Vec next_delta = vec_div_int(vec_mul_int(curr_delta, ttl), ttl + 1);
  return vec_add(dest, next_delta);
}

inline Vec vec_abs(Vec vec){
  return vec_make(fix_abs(vec.x), fix_abs(vec.y));
}

#endif
