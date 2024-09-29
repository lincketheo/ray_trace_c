#include "sphere.h"
#include "hit_result.h"
#include "interval.h"
#include "vec3.h"

hit_result sphere_hit(const sphere s, const ray* r, const interval tslice)
{
  v3 diff = v3s(s.center, r->origin);
  double a = v3d(r->direction, r->direction);
  double b = -2 * v3d(r->direction, diff);
  double c = v3d(diff, diff) - s.radius * s.radius;

  double disc = b * b - 4 * a * c;

  if (disc < 0) {
    return (hit_result) { .hit = 0 };
  }

  double sqrtd = sqrt(disc);
  double root = (-b - sqrtd) / (2 * a);

  if (!interval_contains_open(tslice, root)) {
    root = (-b + sqrtd) / (2 * a);
    if (!interval_contains_open(tslice, root)) {
      return (hit_result) {
        .hit = 0,
      };
    }
  }

  v3 loc = ray_at(r, root);
  v3 out_normal = v3sm(v3s(loc, s.center), 1 / s.radius);

  hit_result ret = { .hit = 1, .t = root, .loc = loc, .m = s.mat };

  hit_result_set_face_normal(&ret, r, out_normal);

  return ret;
}
