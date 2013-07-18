#ifndef WAST_H

#define WAST_H

#include <wmacros.h>
#include <wtype.h>
#include <wtoken.h>
#include <werror.h>

def_struct ( wast ) {
  wtype * type;
  wast * parent;
  wtoken * op;
  };


def_struct ( wast_prefix ) {
  wtype * type;
  wast * parent;
  wtoken_prefix * op;
  wobj * child;
  };


def_struct ( wast_infix ) {
  wtype * type;
  wast * parent;
  wtoken_infix * op;
  wobj * left;
  wobj * right;
  };


def_struct ( wast_list ) {
  wtype * type;
  wast * parent;
  wtoken * op;
  wobj * primary;
  warray_wobj_ptr * children;
  };

wdeclare_composite ( wast );
wdeclare_composite ( wast_prefix );
wdeclare_composite ( wast_infix );
wdeclare_composite ( wast_list );

wast * wast_parent_exprlist ( wast * ast );
wobj * wast_remove_rightmost ( wast * w );
werror * wast_add_rightmost ( wast * w, wobj * o );
wast_list * wast_list_new ( wtoken * op );
void wast_print ( wast * w );

#endif /* end of include guard: WAST_H */
