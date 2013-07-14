#ifndef WTOKEN_H

#define WTOKEN_H

#include <stdint.h>
#include <wtype_h.h>
#include <wstr.h>
#include <wmacros.h>
#include <wobj.h>

#define warray_elem_t wobj_ptr
#define warray_elem_kind pointer
#include <warray.h>


def_struct ( wtoken ) {
  wtype * type;
  wstr * text;
  };

def_struct ( wtoken_infix ) {
  wtype * type;
  wstr * text;
  int lbp;
  int rbp;
  };

def_struct ( wtoken_prefix ) {
  wtype * type;
  wstr * text;
  int rbp;
  };

def_struct ( wtoken_left ) {
  wtype * type;
  wstr * text;
  struct wtoken_right * right;
  };

def_struct ( wtoken_right ) {
  wtype * type;
  wstr * text;
  struct wtoken_left * left;
  };

whash whash_wtoken ( wval );
int wcompare_wtoken ( wval, wval );

wtoken * wtoken_new ( wstr * text );
werror * wtoken_init ( wtoken * self, wstr * text );
wtoken_infix * wtoken_infix_new ( wstr * text, int lbp, int rbp );
wtoken_prefix * wtoken_prefix_new ( wstr * text, int rbp );
wtoken_left * wtoken_left_new ( wstr * text );
wtoken_right * wtoken_right_new ( wstr * text );

wdeclare_composite ( wtoken );
wdeclare_composite ( wtoken_infix );
wdeclare_composite ( wtoken_prefix );
wdeclare_composite ( wtoken_left );
wdeclare_composite ( wtoken_right );

void wtokens_print ( warray_wobj_ptr * self );

#endif /* end of include guard: WTOKEN_H */
