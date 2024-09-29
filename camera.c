#include "camera.h"
#include "hit_result.h"
#include "ppm.h"
#include "ray.h"
#include "utils.h"
#include "vec3.h"
#include <assert.h>
#include <math.h>
#include <omp.h>

camera camera_create_default()
{
  return (camera) { .ar = 1.0,
    .pxw = 100,
    .samples_per_pixel = 10,
    .max_depth = 10,
    .vfov = 90,
    .lookfrom = v3c(0, 0, 0),
    .lookat = v3c(0, 0, -1),
    .vup = v3c(0, 1, 0),
    .defocus_angle = 0,
    .focus_dist = 10 };
}

static void camera_initialize(camera* w)
{
  w->pxh = w->pxw / w->ar;
  assert(w->pxh >= 1);

  w->center = w->lookfrom;

  // Dist between look from and look at
  // double focal_length = v3n2s(v3s(w->lookfrom, w->lookat));
  double theta = w->vfov * M_PI / 180;
  double vph = 2 * tan(theta / 2) * w->focus_dist;
  double vpw = vph * (double)w->pxw / w->pxh;

  w->w = v3n2v(v3s(w->lookfrom, w->lookat));
  w->u = v3n2v(v3cr(w->vup, w->w));
  w->v = v3cr(w->w, w->u);

  v3 viewport_u = v3sm(w->u, vpw);
  v3 viewport_v = v3sm(w->v, -vph);

  w->pixel_delta_u = v3sm(viewport_u, 1.0 / w->pxw);
  w->pixel_delta_v = v3sm(viewport_v, 1.0 / w->pxh);

  v3 vpul = v3s(w->center, v3sm(w->w, w->focus_dist));
  vpul = v3s(vpul, v3sm(viewport_u, 0.5));
  vpul = v3s(vpul, v3sm(viewport_v, 0.5));

  w->pixel00_loc = v3a(vpul, v3sm(v3a(w->pixel_delta_u, w->pixel_delta_v), 0.5));

  double defocus_radius = w->focus_dist * tan((w->defocus_angle / 2) * M_PI / 180);
  w->defocus_disk_u = v3sm(w->u, defocus_radius);
  w->defocus_disk_v = v3sm(w->v, defocus_radius);
}

static ray random_ray_to_pixel(const camera* w, size_t row, size_t col)
{
  // Pick a random spot inside the pixel to emit the ray
  v3 rand_offset = v3c(randd() - 0.5, randd() - 0.5, 0);
  v3 pixel_sample = v3a(w->pixel00_loc, v3sm(w->pixel_delta_u, row + rand_offset.x));
  pixel_sample = v3a(pixel_sample, v3sm(w->pixel_delta_v, col + rand_offset.y));

  v3 origin;
  if (w->defocus_angle <= 0) {
    origin = w->center;
  } else {

    // Pick a random spot on the viewing unit disk to emit the ray
    v3 p = v3rand_unit_disk();
    origin = v3a(w->center, v3sm(w->defocus_disk_u, p.x));
    origin = v3a(origin, v3sm(w->defocus_disk_v, p.y));
  }

  v3 dir = v3s(pixel_sample, origin);

  return (ray) { .origin = origin, .direction = dir };
}

static v3 ray_color(const ray* r, const obj_collection* objs, int max_depth)
{
  if (max_depth <= 0)
    return v3c(0, 0, 0);

  hit_result hr = objc_hit(objs, r, interval_c(0.001, DBL_MAX));
  if (hr.hit) {
    scatter_result res = hr_scatter(&hr, r);
    if (res.did_scatter) {
      v3 color = ray_color(&res.scattered, objs, max_depth - 1);
      return v3hp(res.attenuation, color);
    }
    return v3c(0, 0, 0);
  }
  v3 unit_d = v3n2v(r->direction);
  double a = 0.5 * (unit_d.y + 1.0);
  return v3a(v3sm(v3c(1, 1, 1), 1.0 - a), v3sm((v3)v3c(0.5, 0.7, 1.0), a));
}

void camera_render_ppm(camera* w, const obj_collection* objs)
{
  assert(w);
  assert(objs);
  camera_initialize(w);

  ppm p = ppm_start_hw(stdout, w->pxh, w->pxw);

  int rows_complete = 0;

  v3* img_data = malloc(p.height * p.width * sizeof *img_data);

#pragma omp parallel for
  for (int j = 0; j < p.height; ++j) {
    for (int i = 0; i < p.width; ++i) {
      v3 color = v3c(0, 0, 0);
      for (int s = 0; s < w->samples_per_pixel; ++s) {
        ray r = random_ray_to_pixel(w, i, j);
        color = v3a(color, ray_color(&r, objs, w->max_depth));
      }
      color = v3sm(color, 1.0 / w->samples_per_pixel);
      img_data[j * p.width + i] = color;
    }

#pragma omp critical
    {
      rows_complete++;
      fprintf(stderr, "\rRows complete pcnt: %.3f%%", 100.0 * (double)rows_complete / p.height);
    }
  }

  for (int j = 0; j < p.height; ++j) {
    for (int i = 0; i < p.width; ++i) {
      ppm_write_color(&p, img_data[j * p.width + i]);
    }
  }
  free(img_data);

  fprintf(stderr, "\rDone                                                        \n");
}
