#ifndef WICK_CORE_H
#define WICK_CORE_H

#include "wick_basic.h"

struct wick_target;
struct wick_place;

struct wick_core {
  struct wick_place * global_place;
};

typedef struct wick_core wick_core;

wick_core * wick_core_new(NOARGS);

#endif /* WICK_CORE_H */
