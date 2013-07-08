#ifndef WAST_H

#define WAST_H

#include <wmacros.h>
#include <wtype.h>
#include <wtoken.h>

#define warray_elem_t wobj_ptr
#define warray_elem_kind pointer
#include <warray.h>

def_struct ( wast ) {
  wtype * type;
  wast * parent;
  wtoken * op;
  };


def_struct ( wast_unop ) {
  wtype * type;
  wast * parent;
  wtoken * op;
  wobj * child;
  };


def_struct ( wast_binop ) {
  wtype * type;
  wast * parent;
  wtoken * op;
  wobj * left;
  wobj * right;
  };


def_struct ( wast_list ) {
  wtype * type;
  wast * parent;
  wtoken * op;
  warray_wobj_ptr * children;
  };

wdeclare_composite ( wast_unop );
wdeclare_composite ( wast_binop );
wdeclare_composite ( wast_list );

#endif /* end of include guard: WAST_H */
