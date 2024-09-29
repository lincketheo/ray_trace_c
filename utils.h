#pragma once

#include <stdlib.h>

// [0, 1)
static inline double randd() { return rand() / (RAND_MAX + 1.0); }

// [min, max)
static inline double randdr(double min, double max) {
  return min + (max - min) * randd();
}
