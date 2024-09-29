#pragma once

#include "vec3.h"
#include <stdio.h>

typedef struct {
  FILE *ofp;
  size_t height;
  size_t width;
  size_t state;
} ppm;

static inline double ppm_ar(ppm *p) {
  return (double)p->width / (double)p->height;
}

void ppm_write_color(ppm *p, v3 color);

ppm ppm_start_hw(FILE *ofp, size_t height, size_t width);

ppm ppm_start_wa(FILE *ofp, size_t width, double ar);
