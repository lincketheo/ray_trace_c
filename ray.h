#pragma once

#include "vec3.h"

typedef struct {
  v3 origin;
  v3 direction;
} ray;

#define rayc(o, d) ((ray){.origin = o, .direction = d})

static inline v3 ray_at(const ray *r, double t) {
  return v3a(v3sm(r->direction, t), r->origin);
}
