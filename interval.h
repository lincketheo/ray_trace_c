#pragma once

#include <float.h>

typedef struct {
  double min;
  double max;
} interval;

#define interval_c(_min, _max) ((interval){.min = _min, .max = _max})

static inline int interval_contains_closed(const interval i, double v) {
  return i.min <= v && v <= i.max;
}

static inline int interval_contains_open(const interval i, double v) {
  return i.min < v && v < i.max;
}

static inline double interval_size(const interval i) { return i.max - i.min; }

static inline double interval_clamp(const interval i, double v) {
  if (v < i.min)
    return i.min;
  if (v > i.max)
    return i.max;
  return v;
}

static const interval interval_empty = interval_c(DBL_MAX, DBL_MIN);
static const interval interval_universe = interval_c(DBL_MIN, DBL_MAX);
