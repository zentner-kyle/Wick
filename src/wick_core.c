#include "wick_core.h"
#include "wick_place.h"
#include "wick_alloc.h"

static wick_core * wick_core_init(wick_place * env, wick_core * core) {
  if (!core) {
    // Allocation failed.
    return 0;
  }
  wick_place_init(NULL, core->global_place);
  return core;
}

wick_core * wick_core_new(NOARGS) {
  wick_core * core = WICK_ALLOC(wick_core);
  wick_place * global = WICK_ALLOC(wick_place);
  if (core && global) {
    core->global_place = global;
    return wick_core_init(NULL, core);
  } else {
    wick_host_free(core);
    wick_host_free(global);
    return NULL;
  }
}
