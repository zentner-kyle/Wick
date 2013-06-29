#include <wtype.h>

void wtype_init_header ( wtype * t ) {
  }

void wtype_init_base ( wtype_base * t, wstr name, size_t size ) {
  wtype_init_header ( ( wtype * )t );
  t->size = size;
  t->id = wsym_of_wstr ( name );
  }

wdefine_composite ( wtype );

wtype * wtype_wtype_alien;
wtype * wtype_wtype_base;
wtype * wtype_wtype_composite;
wtype * wtype_wtype_dynamic;
wtype * wtype_wtype_pointer;
wtype * wtype_wtype_variant;
wtype * wtype_wtype_parametric;
wtype * wtype_wtype_abstract;

