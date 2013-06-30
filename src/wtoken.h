#ifndef WTOKEN_H

#define WTOKEN_H

#include <stdint.h>
#include <wtype_h.h>
#include <wstr.h>
#include <wmacros.h>

def_struct ( wtoken ) {
  wtype * type;
  int family;
  wstr * text;
  int lbp;
  int rbp;
  };

whash whash_wtoken ( wval );
int wcompare_wtoken ( wval, wval );

wtoken * wtoken_new ( int family, wstr * text, int lbp, int rbp );

wdeclare_composite ( wtoken );

#endif /* end of include guard: WTOKEN_H */
