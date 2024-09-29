#include "hit_result.h"
#include "vec3.h"
#include <assert.h>

void hit_result_set_face_normal(hit_result *res, const ray *r,
                                const v3 out_normal) {
  assert(res);
  assert(r);

  res->front_face = v3d(r->direction, out_normal) < 0;
  res->normal = res->front_face ? out_normal : v3sm(out_normal, -1);
}

scatter_result lambertian_scatter(lambertian *l, ray *ray_in, hit_result *hr) {
  v3 scatter_direction = v3a(hr->normal, v3randdu());

  // degenerate scatter directions
  if (v3_near_zero(scatter_direction))
    scatter_direction = hr->normal;

  ray scattered = rayc(hr->loc, scatter_direction);
  return (scatter_result){
      .scattered = scattered, .did_scatter = 1, .attenuation = l->albedo};
}

scatter_result metal_scatter(metal *l, ray *ray_in, hit_result *hr) {
  v3 reflected = v3reflect(ray_in->direction, hr->normal);
  reflected = v3a(v3n2v(reflected), v3sm(v3randdu(), l->fuzz));
  ray scattered = rayc(hr->loc, reflected);
  return (scatter_result){.scattered = scattered,
                          .did_scatter =
                              v3d(scattered.direction, hr->normal) > 0,
                          .attenuation = l->albedo};
}

double reflectance(double cosine, double ri) {
  double r0 = (1 - ri) / (1 + ri);
  r0 = r0 * r0;
  return r0 + (1 - r0) * pow((1 - cosine), 5);
}

scatter_result dielectric_scatter(dielectric *l, ray *ray_in, hit_result *hr) {
  v3 attenuation = v3c(1.0, 1.0, 1.0);
  double ri =
      hr->front_face ? (1.0 / l->refraction_index) : l->refraction_index;
  v3 unit_dir = v3n2v(ray_in->direction);

  double cos_theta = fmin(v3d(v3sm(unit_dir, -1), hr->normal), 1.0);
  double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

  int cannot_refract = ri * sin_theta > 1.0;
  v3 direction;

  if (cannot_refract || reflectance(cos_theta, ri) > randd())
    direction = v3reflect(unit_dir, hr->normal);
  else
    direction = v3refract(unit_dir, hr->normal, ri);

  ray scattered = rayc(hr->loc, direction);

  return (scatter_result){
      .attenuation = attenuation,
      .scattered = scattered,
      .did_scatter = 1,
  };
}
