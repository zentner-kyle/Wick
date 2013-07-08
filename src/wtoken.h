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

whash whash_wtoken ( wval );
int wcompare_wtoken ( wval, wval );

wtoken * wtoken_new ( int family, wstr * text );
wtoken * wtoken_new_op ( int family, wstr * text, int lbp, int rbp );

wdeclare_composite ( wtoken );

void wtokens_print ( warray_wobj_ptr * self );

#endif /* end of include guard: WTOKEN_H */
