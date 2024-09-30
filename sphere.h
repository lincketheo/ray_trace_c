#pragma once

#include "hit_result.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"

typedef struct {
  v3 center;
  double radius;
  material mat;
} sphere;

#define sphere_from_xyzrm(x, y, z, r, m) \
  ((sphere) { .center = v3c(x, y, z), .radius = r, .mat = m })

hit_result sphere_hit(const sphere s, const ray* r, const interval tslice);
