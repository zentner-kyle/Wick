#include <wick_dyn.h>

wick_dyn wick_make_dyn ( void * obj, struct wick_type * type ) {
  return ( wick_dyn ) {obj, type  };
  }
