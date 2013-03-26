#include "wtype.h"

void wtype_init_header(wtype * t) {
}

void wtype_init_base(wtype_base * t, wstr name, size_t size) {
	wtype_init_header((wtype *)t);
	t->size = size;
	t->id = wsym_of_wstr(name);
}

