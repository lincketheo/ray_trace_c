#include "ppm.h"
#include "exception.h"
#include "interval.h"
#include <assert.h>

double linear_to_gamma(double l) {
  if (l > 0)
    return sqrt(l);
  return 0;
}

void ppm_write_color(ppm *p, v3 color) {
  assert(p);
  assert(p->ofp);
  assert(p->state < p->height * p->width);
  assert(color.x >= 0 && color.x <= 1);
  assert(color.y >= 0 && color.y <= 1);
  assert(color.z >= 0 && color.z <= 1);

  static const interval intensity = interval_c(0, 0.999);
  int r = 256 * interval_clamp(intensity, linear_to_gamma(color.x));
  int g = 256 * interval_clamp(intensity, linear_to_gamma(color.y));
  int b = 256 * interval_clamp(intensity, linear_to_gamma(color.z));

  try_or_exit(fprintf(p->ofp, "%d %d %d\n", r, g, b) < 0, "fprintf");
  p->state++;
}

ppm ppm_start_hw(FILE *ofp, size_t height, size_t width) {
  assert(ofp);
  assert(height > 0);
  assert(width > 0);
  try_or_exit(fprintf(ofp, "P3\n%zu %zu\n255\n", width, height) < 0, "fprintf");
  ppm ret;
  ret.ofp = ofp;
  ret.height = height;
  ret.width = width;
  ret.state = 0;
  return ret;
}

ppm ppm_start_wa(FILE *ofp, size_t width, double ar) {
  return ppm_start_hw(ofp, (size_t)((double)width / ar), width);
}
