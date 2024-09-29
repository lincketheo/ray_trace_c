#pragma once

#include "hit_result.h"
#include "interval.h"
#include "sphere.h"
#include <stdlib.h>

#define MAX_SPHERES 1000

typedef struct {
  sphere spheres[MAX_SPHERES];
  size_t slen;
} obj_collection;

obj_collection objc_create();

hit_result objc_hit(const obj_collection *s, const ray *r,
                    const interval tslice);

int objc_add_sphere(obj_collection *w, const sphere s);
