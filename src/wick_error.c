#include "wick_error.h"
#include "wick_place.h"
#include "wick_globals.h"

void wick_signal_alloc_error(wick_place * place) {
  wick_place_signal_error(place, &wick_global.error.alloc);
}
