#pragma once

#include <stdio.h>
#include <stdlib.h>

static inline void try_or_exit(int expr, const char *perr_msg) {
  if (expr) {
    perror(perr_msg);
    exit(-1);
  }
}
