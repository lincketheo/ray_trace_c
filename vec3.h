#pragma once

#include "utils.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  double x;
  double y;
  double z;
} v3;

static inline void v3print(const v3 v) {
  printf("[%f %f %f]\n", v.x, v.y, v.z);
}

// vec3 constructor
#define v3c(_x, _y, _z) ((v3){.x = _x, .y = _y, .z = _z})

// vec3 add
static inline v3 v3a(const v3 left, const v3 right) {
  return (v3){
      .x = left.x + right.x, .y = left.y + right.y, .z = left.z + right.z};
}

// vec3 subtract
static inline v3 v3s(const v3 left, const v3 right) {
  return (v3){
      .x = left.x - right.x, .y = left.y - right.y, .z = left.z - right.z};
}

// vec3 scalar multiply
static inline v3 v3sm(const v3 vec, const double scl) {
  return (v3){.x = vec.x * scl, .y = vec.y * scl, .z = vec.z * scl};
}

// vec3 2 norm scalar squared
static inline double v3n2ss(const v3 vec) {
  return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;
}

// vec3 2 norm scalar
static inline double v3n2s(const v3 vec) { return sqrt(v3n2ss(vec)); }

// vec3 dot
static inline double v3d(const v3 left, const v3 right) {
  return left.x * right.x + left.y * right.y + left.z * right.z;
}

// vec3 2 norm vec3
static inline v3 v3n2v(const v3 dest) { return v3sm(dest, 1 / v3n2s(dest)); }

// left and right face the same direction
static inline int v3_same_dir(const v3 left, const v3 right) {
  return v3d(left, right) > 0.0;
}

// vec3 rand double
static inline v3 v3randd() { return v3c(randd(), randd(), randd()); }

// vec3 rand double range
static inline v3 v3randdr(double min, double max) {
  return v3c(randdr(min, max), randdr(min, max), randdr(min, max));
}

// vec3 rand double unit
static inline v3 v3randdu() {
  while (1) {
    v3 p = v3randdr(-1, 1);
    double l = v3n2ss(p);
    if (l <= 1 && l > 1e-160) // reject outside sphere and tiny (to avoid div 0)
      return v3sm(p, 1 / sqrt(l));
  }
}

// vec3 rand double unit away from surface
static inline v3 v3randdus(const v3 normal) {
  v3 ret = v3randdu();
  if (v3d(normal, ret) > 0.0)
    return ret;
  return v3sm(ret, -1);
}

static inline int v3_near_zero(const v3 v) {
  double s = 1e-8;
  return fabs(v.x) < s && fabs(v.y) < s && fabs(v.z) < s;
}

static inline v3 v3reflect(const v3 v, const v3 n) {
  return v3s(v, v3sm(n, 2 * v3d(v, n)));
}

// vec3 hadamard product
static inline v3 v3hp(const v3 left, const v3 right) {
  return v3c(left.x * right.x, left.y * right.y, left.z * right.z);
}

static inline v3 v3refract(const v3 uv, const v3 n, double etai_over_etat) {
  double cos_theta = fmin(v3d(v3sm(uv, -1), n), 1.0);
  v3 r_out_perp = v3sm(v3a(uv, v3sm(n, cos_theta)), etai_over_etat);
  v3 r_out_parallel = v3sm(n, -sqrt(fabs(1.0 - v3n2ss(r_out_perp))));
  return v3a(r_out_perp, r_out_parallel);
}

static inline v3 v3cr(const v3 u, const v3 v) {
  return v3c(u.y * v.z - u.z * v.y, u.z * v.x - u.x * v.z,
             u.x * v.y - u.y * v.x);
}

static inline v3 v3rand_unit_disk() {
  while (1) {
    v3 p = v3c(randdr(-1, 1), randdr(-1, 1), 0);
    if (v3n2ss(p) < 1)
      return p;
  }
}
