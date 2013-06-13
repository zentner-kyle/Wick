#include "wcall.h"
#include <stdlib.h>

void winvoke ( wcall c ) {
  if (c.func) {
    c.func( c.data );
  }
}

wcall static_wcall( wcall_func_t func, wcall_data_t data ) {
  wcall c = {func, data};
  return c;
}

const wcall null_wcall = {NULL, NULL};
