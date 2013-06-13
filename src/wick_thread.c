#include <wick_thread.h>
#include <type_tags.h>

wick_thread * wick_thread_init ( wick_thread * p, wick_thread * t ) {
  t->id = WICK_THREAD_TAG;
  t->parent = p;
  return t;
  }

wick_thread * wick_thread_dyn ( wick_dyn obj ) {
  if ( wick_dyn_is_thread ( obj ) ) {
    return ( wick_thread * )DYN ( obj );
    }
  else {
    return 0;
    }
  }
