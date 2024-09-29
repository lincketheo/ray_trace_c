#include "obj_collection.h"
#include "hit_result.h"
#include "interval.h"
#include "sphere.h"

#include <assert.h>

obj_collection objc_create() { return (obj_collection) { .slen = 0 }; }

hit_result objc_hit(const obj_collection* s, const ray* r,
    const interval tsclice)
{
  assert(s);
  assert(r);

  hit_result ret = {
    .hit = 0,
    .t = tsclice.max,
  };

  for (int i = 0; i < s->slen; ++i) {
    hit_result res = sphere_hit(s->spheres[i], r, interval_c(tsclice.min, ret.t));
    if (res.hit) {
      ret = res;
    }
  }

  return ret;
}

int objc_add_sphere(obj_collection* w, const sphere s)
{
  assert(w);
  if (w->slen == MAX_SPHERES)
    return -1;

  w->spheres[w->slen++] = s;
  return 0;
}
