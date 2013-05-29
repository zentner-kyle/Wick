#ifndef WTYPE_H
#define WTYPE_H
#include "wmacros.h"
#include "stdlib.h"
#include "warray.h"
#include "wtable.h"
#include "wsym.h"
#include "wtype_h.h"

#define wtype_upcast( ptr_to_subtype ) ( ( wtype * )( ptr_to_subtype ) )

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

void wtype_init_header(wtype * t);

void wtype_init_base(wtype_base * t, wstr name, size_t size);

#endif /* end of include guard: WTYPE_H */
