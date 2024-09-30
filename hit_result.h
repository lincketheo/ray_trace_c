#pragma once

#include "material.h"
#include "ray.h"
#include "scatter.h"
#include "vec3.h"

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
static inline void hit_result_set_face_normal(hit_result* res, const ray* r,
    const v3 out_normal)
{
  res->front_face = v3d(r->direction, out_normal) < 0;
  res->normal = res->front_face ? out_normal : v3sm(out_normal, -1);
}

/**
 * Choose which scatter to run
 */
static inline scatter_result hr_scatter(const hit_result* hr, const ray* ray_in)
{
  switch (hr->m.t) {
  case LAMB:
    return lambertian_scatter(&hr->m.l, ray_in, hr->loc, hr->normal);
  case METAL:
    return metal_scatter(&hr->m.m, ray_in, hr->loc, hr->normal);
  case DIELEC:
    return dielectric_scatter(&hr->m.d, ray_in, hr->loc, hr->normal, hr->front_face);
  }
}
