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
  int family;
  wstr * text;
  int lbp;
  int rbp;
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

def_struct ( wtoken_postfix ) {
  wtype * type;
  wstr * text;
  int lbp;
  };

def_struct ( wtoken_left ) {
  wtype * type;
  wstr * text;
  struct wtoken_right * right;
  };

def_struct ( wtoken_right ) {
  wtype * type;
  wstr * text;
  struct wtoken_left * right;
  };

whash whash_wtoken ( wval );
int wcompare_wtoken ( wval, wval );

wtoken * wtoken_new ( int family, wstr * text );
wtoken * wtoken_atom_new ( wstr * text );
wtoken * wtoken_infix_new ( wstr * text, int lbp, int rbp );
wtoken * wtoken_prefix_new ( wstr * text, int rbp );
wtoken * wtoken_postfix_new ( wstr * text, int lbp );
wtoken * wtoken_left_new ( wstr * text, wtoken_right * right );
wtoken * wtoken_right_new ( wstr * text, wtoken_left * left );

wtoken * wtoken_new_op ( int family, wstr * text, int lbp, int rbp );

wdeclare_composite ( wtoken );

void wtokens_print ( warray_wobj_ptr * self );

#endif /* end of include guard: WTOKEN_H */
