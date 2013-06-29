#ifndef WTYPE_H
#define WTYPE_H
#include <wmacros.h>
#include <stdlib.h>
#include <warray_wval.h>
#include <wtable_wval.h>
#include <wsym.h>
#include <wstr.h>
#include <wtype_h.h>
#include <wbuiltins.h>

#define wtype_upcast( ptr_to_subtype ) ( ( wtype * ) ( ptr_to_subtype ) )

wdeclare_composite ( wtype );

#define wtable_key_t wstr_ptr
#define wtable_key_kind pointer
#define wtable_val_t wtype_ptr
#define wtable_val_kind pointer
#include <wtable.h>

#define wtable_key_t wstr_ptr
#define wtable_key_kind pointer
#define wtable_val_t size_t
#define wtable_val_kind uinteger
#include <wtable.h>


struct wtype {
  wtype * type;
  wsym id;
  size_t size;
  };

struct wtype_alien {
  wtype * type;
  wsym id;
  size_t size;
  };

struct wtype_base {
  wtype * type;
  wsym id;
  size_t size;
  };

struct wtype_composite {
  wtype * type;
  wsym id;
  size_t size;
  wtable_wstr_ptr_to_wtype_ptr * names_to_types;
  wtable_wstr_ptr_to_size_t * names_to_offsets;
  };

struct wtype_dynamic {
  wtype * type;
  wsym id;
  size_t size;
  };

struct wtype_pointer {
  wtype * type;
  wsym id;
  size_t size;
  wtype * subtype;
  };

struct wtype_variant {
  wtype * type;
  wsym id;
  size_t size;
  warray variants;
  };

struct wtype_variable {
  wtype * type;
  wsym id;
  size_t size;
  wsym variable_name;
  };

struct wtype_parametric {
  wtype * type;
  wsym id;
  size_t size;
  wtable variables;
  };

struct wtype_abstract {
  wtype * type;
  wsym id;
  size_t size;
  wtype * subtype;
  };

void wtype_init_header ( wtype * t );

void wtype_init_base ( wtype_base * t, wstr name, size_t size );

extern wtype * wtype_wtype_alien;
extern wtype * wtype_wtype_base;
extern wtype * wtype_wtype_composite;
extern wtype * wtype_wtype_dynamic;
extern wtype * wtype_wtype_pointer;
extern wtype * wtype_wtype_variant;
extern wtype * wtype_wtype_parametric;
extern wtype * wtype_wtype_abstract;

#endif /* end of include guard: WTYPE_H */
