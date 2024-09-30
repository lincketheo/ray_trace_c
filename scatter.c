#include "scatter.h"

scatter_result metal_scatter(const metal* l, const ray* ray_in, v3 loc, v3 normal)
{
  v3 reflected = v3reflect(ray_in->direction, normal);
  reflected = v3a(v3n2v(reflected), v3sm(v3randdu(), l->fuzz));
  ray scattered = rayc(loc, reflected);
  return (scatter_result) { .scattered = scattered,
    .did_scatter = v3d(scattered.direction, normal) > 0,
    .attenuation = l->albedo };
}

scatter_result lambertian_scatter(const lambertian* l, const ray* ray_in, v3 loc, v3 normal)
{
  v3 scatter_direction = v3a(normal, v3randdu());

  // degenerate scatter directions
  if (v3_near_zero(scatter_direction))
    scatter_direction = normal;

  ray scattered = rayc(loc, scatter_direction);
  return (scatter_result) {
    .scattered = scattered, .did_scatter = 1, .attenuation = l->albedo
  };
}

static double reflectance(double cosine, double ri)
{
  double r0 = (1 - ri) / (1 + ri);
  r0 = r0 * r0;
  return r0 + (1 - r0) * pow((1 - cosine), 5);
}

scatter_result dielectric_scatter(const dielectric* l, const ray* ray_in, v3 loc, v3 normal, int front_face)
{
  v3 attenuation = v3c(1.0, 1.0, 1.0);
  double ri = front_face ? (1.0 / l->refraction_index) : l->refraction_index;
  v3 unit_dir = v3n2v(ray_in->direction);

  double cos_theta = fmin(v3d(v3sm(unit_dir, -1), normal), 1.0);
  double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

  int cannot_refract = ri * sin_theta > 1.0;
  v3 direction;

  if (cannot_refract || reflectance(cos_theta, ri) > randd())
    direction = v3reflect(unit_dir, normal);
  else
    direction = v3refract(unit_dir, normal, ri);

  ray scattered = rayc(loc, direction);

  return (scatter_result) {
    .attenuation = attenuation,
    .scattered = scattered,
    .did_scatter = 1,
  };
}
