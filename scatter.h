#pragma once

#include "vec3.h"
#include "ray.h"
#include "material.h"

typedef struct {
  int did_scatter;
  ray scattered;
  v3 attenuation;
} scatter_result;

/**
 * Scatter implementation of lambertian 
 *
 * A Lambertian assigns higher probability of 
 * scattering closer to the normal and smaller 
 * probability as angle grows away from normal
 */
scatter_result lambertian_scatter(const lambertian *l, const ray *ray_in, v3 loc, v3 normal);

/**
 * Metal reflects. See v3reflect
 */
scatter_result metal_scatter(const metal *l, const ray *ray_in, v3 loc, v3 normal);

/**
 * Dielectrics reflect and refract
 */
scatter_result dielectric_scatter(const dielectric *l, const ray *ray_in, v3 loc, v3 normal, int front_face);
