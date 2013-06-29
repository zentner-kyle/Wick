#ifndef WVAL_H

#define WVAL_H
#include <stdint.h>

#include <wobj.h>
#include <wmacros.h>

def_union ( wval ) {
  int64_t integer;
  uint64_t uinteger;
  double floating;
  wobj * pointer;
  };

wdeclare_base ( wval );


#endif /* end of include guard: WVAL_H */
