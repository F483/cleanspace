// Copyright (c) 2014 Fabian Barkhau <fabian.barkhau@gmail.com> 
// License: MIT (see LICENSE.TXT file)  

#ifndef GBX_COLLISION_H
#define GBX_COLLISION_H

#include <src/gbx/common.h>
#include <src/gbx/lib/fix.h>
#include <src/gbx/lib/vec.h>

////////////
// CIRCLE //
////////////

inline Bool collision_circle_check(Vec pos_a, Fix rad_a, Vec pos_b, Fix rad_b){
  Bool result = false;
  Fix contact_dist = fix_add(rad_a, rad_b);
  Vec delta = vec_abs(vec_sub(pos_a, pos_b));
  // only check if close to avoid integer overflows in vec_distance_sqrd
  if (fix_lte(vec_get_x(delta), contact_dist) and 
      fix_lte(vec_get_y(delta), contact_dist)){
    Fix contact_dist_sqrd = fix_mul64(contact_dist, contact_dist);
    Fix object_dist_sqrd = vec_distance_sqrd(pos_a, pos_b);
    result = fix_lte(object_dist_sqrd, contact_dist_sqrd);
  }
  return result;
}

inline void collision_circle_bounce(Vec pos_a, Vec* dir_a, 
                                    Fix rad_a, Fix mass_a,
                                    Vec pos_b, Vec* dir_b, 
                                    Fix rad_b, Fix mass_b){

  // minimum translation distance to push balls apart after intersecting
  Vec delta = vec_sub(pos_a, pos_b);
  Fix d = vec_length(delta);
  Fix distance_ab = fix_add(rad_a, rad_b);
  Vec mtd = vec_mul_fix(delta, fix_div64(fix_sub(distance_ab, d), d));

  // impact speed
  Vec v = vec_sub(*dir_a, *dir_b);
  Fix vn = vec_dot(v, vec_normalize(mtd));

  // sphere intersecting but moving away from each other already
  if (fix_gt(vn, fix_zero)) return;

  // inverse mass quantities
  Fix im1 = fix_div64(fix_one, mass_a);
  Fix im2 = fix_div64(fix_one, mass_b);
  Fix im = fix_add(im1, im2);

  // collision impulse
  // Fix restitution = fix_zero; 
  // Fix factor = fix_neg(fix_add(fix_one, restitution));
  Fix factor = float_to_fix(-0.5); // TODO understand the math of this code
  Fix i = fix_div64(fix_mul64(factor, vn), im);
  Vec impulse = vec_mul_fix(mtd, i);

  // change in momentum
  *dir_a = vec_add(*dir_a, vec_mul_fix(impulse, im1));
  *dir_b = vec_sub(*dir_b, vec_mul_fix(impulse, im2));
}

/**
 * Source: http://stackoverflow.com/questions/345838
 * returns true if collision was detected and dir_a/dir_b changed.
 */
inline Bool collision_circle(Vec pos_a, Vec* dir_a, Fix rad_a, Fix mass_a,
                             Vec pos_b, Vec* dir_b, Fix rad_b, Fix mass_b){
  if (collision_circle_check(pos_a, rad_a, pos_b, rad_b)){
    collision_circle_bounce(pos_a, dir_a, rad_a, mass_a, 
                            pos_b, dir_b, rad_b, mass_b);
    return true;
  }
  return false;
}

#endif
