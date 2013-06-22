#ifndef WVAL_H

#define WVAL_H
#include <stdint.h>

#include <wobj.h>
#include <wmacros.h>

def_union ( wval ) {
  int64_t i;
  uint64_t u;
  double f;
  wobj * p;
  };


#endif /* end of include guard: WVAL_H */
