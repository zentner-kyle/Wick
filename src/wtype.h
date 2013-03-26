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
	#include "wobj_header.h"
	wsym id;
	size_t size;
	wtype * meta_type;
};

struct wtype_alien {
	#include "wobj_header.h"
	wsym id;
	size_t size;
	wtype * meta_type;
};

struct wtype_base {
	#include "wobj_header.h"
	wsym id;
	size_t size;
	wtype * meta_type;
};

struct wtype_composite {
	#include "wobj_header.h"
	wsym id;
	size_t size;
	wtype * meta_type;
	warray fields;
	wtable names;
};

struct wtype_dynamic {
	#include "wobj_header.h"
	wsym id;
	size_t size;
	wtype * meta_type;
};

struct wtype_ptr {
	#include "wobj_header.h"
	wsym id;
	size_t size;
	wtype * meta_type;
	wtype * subtype;
};

struct wtype_variant {
	#include "wobj_header.h"
	wsym id;
	size_t size;
	wtype * meta_type;
	warray variants;
};

void wtype_init_header(wtype * t);

void wtype_init_base(wtype_base * t, wstr name, size_t size);

#endif /* end of include guard: WTYPE_H */
