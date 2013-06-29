#ifndef WTYPE_H
#define WTYPE_H
#include <wmacros.h>
#include <stdlib.h>
#include <warray_wval.h>
#include <wtable_wval.h>
#include <wsym.h>
#include <wtype_h.h>

#define wtype_upcast( ptr_to_subtype ) ( ( wtype * ) ( ptr_to_subtype ) )

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
  warray fields;
  wtable names;
  };

struct wtype_dynamic {
  wtype * type;
  wsym id;
  size_t size;
  };

struct wtype_ptr {
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

struct wtype_cfunc {
  wtype * type;
  wsym id;
  size_t size;
  warray arg_names;
  warray arg_types;
  };

void wtype_init_header ( wtype * t );

void wtype_init_base ( wtype_base * t, wstr name, size_t size );

extern wtype * wtype_wtype;
extern wtype * wtype_wtype_alien;
extern wtype * wtype_wtype_base;
extern wtype * wtype_wtype_composite;
extern wtype * wtype_wtype_dynamic;
extern wtype * wtype_wtype_ptr;
extern wtype * wtype_wtype_variant;
extern wtype * wtype_wtype_parametric;
extern wtype * wtype_wtype_abstract;
extern wtype * wtype_wtype_cfunc;

#endif /* end of include guard: WTYPE_H */
