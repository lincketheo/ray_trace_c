#pragma once

#include "ray.h"
#include "vec3.h"

typedef enum { LAMB, METAL, DIELEC } material_t;

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
#define mat_lambc(x, y, z)                                                     \
  ((material){.t = LAMB, .l = (lambertian){.albedo = v3c(x, y, z)}})

#define mat_metalc(x, y, z, f)                                                 \
  ((material){.t = METAL, .m = (metal){.albedo = v3c(x, y, z), .fuzz = f}})

#define mat_dielecc(ri)                                                        \
  ((material){.t = DIELEC, .d = (dielectric){.refraction_index = ri}})

typedef struct {
  int did_scatter;
  ray scattered;
  v3 attenuation;
} scatter_result;

typedef struct {
  int hit;
  double t;
  v3 normal;
  v3 loc;
  int front_face;
  material m;
} hit_result;


/**
 * Check if the angle between ray [r] and [out_normal] is > 90 
 * (meaning they're pointing in "opposite directions"
 * If so, set hit result normal to -out_normal (so that normal 
 * points towards the ray)
 */
void hit_result_set_face_normal(hit_result *res, const ray *r,
                                const v3 out_normal);

/**
 * Scatter implementation of lambertian 
 *
 * A Lambertian assigns higher probability of 
 * scattering closer to the normal and smaller 
 * probability as angle grows away from normal
 */
scatter_result lambertian_scatter(lambertian *l, ray *ray_in, hit_result *hr);

/**
 * Metal reflects. See v3reflect
 */
scatter_result metal_scatter(metal *l, ray *ray_in, hit_result *hr);

/**
 * Dielectrics reflect and refract
 */
scatter_result dielectric_scatter(dielectric *l, ray *ray_in, hit_result *hr);

/**
 * Choose which scatter to run
 */
static inline scatter_result hr_scatter(hit_result *hr, ray *ray_in) {
  switch (hr->m.t) {
  case LAMB:
    return lambertian_scatter(&hr->m.l, ray_in, hr);
  case METAL:
    return metal_scatter(&hr->m.m, ray_in, hr);
  case DIELEC:
    return dielectric_scatter(&hr->m.d, ray_in, hr);
  }
}
