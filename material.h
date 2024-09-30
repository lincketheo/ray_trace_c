#pragma once

#include "ray.h"
#include "vec3.h"

typedef enum {
  LAMB,
  METAL,
  DIELEC,
} material_t;

typedef struct {
  v3 albedo;
} lambertian;

typedef struct {
  v3 albedo;
  double fuzz;
} metal;

typedef struct {
  double refraction_index;
} dielectric;

typedef struct {
  material_t t;
  union {
    metal m;
    lambertian l;
    dielectric d;
  };
} material;

// Constructors
#define mat_lamb_from_xyz(x, y, z) \
  ((material) { .t = LAMB, .l = (lambertian) { .albedo = v3c(x, y, z) } })

#define mat_metal_from_xyzf(x, y, z, f) \
  ((material) { .t = METAL, .m = (metal) { .albedo = v3c(x, y, z), .fuzz = f } })

#define mat_dielec_from_ri(ri) \
  ((material) { .t = DIELEC, .d = (dielectric) { .refraction_index = ri } })
