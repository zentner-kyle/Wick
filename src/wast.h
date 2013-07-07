#ifndef WAST_H

#define WAST_H

#include <wmacros.h>
#include <wtype.h>
#include <wtoken.h>

def_struct ( wast_unop ) {
  wtype * type;
  wobj * parent;
  wtoken * op;
  wobj * child;
  };


def_struct ( wast_binop ) {
  wtype * type;
  wobj * parent;
  wtoken * op;
  wobj * left;
  wobj * right;
  };


def_struct ( wast_list ) {
  wtype * type;
  wobj * parent;
  wtoken * op;
  warray_wobj_ptr * children;
  };

wdeclare_composite ( wast_unop );
wdeclare_composite ( wast_binop );
wdeclare_composite ( wast_list );

#endif /* end of include guard: WAST_H */
