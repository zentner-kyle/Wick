#include <wmacros.h>

def_struct ( wtoken ) {
  wtype * type;
  uint32_t family;
  wstr text;
  };

whash whash_wtoken ( wval );
int wcompare_wtoken ( wval, wval );
