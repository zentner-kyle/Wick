#ifndef WSTR_H
#define WSTR_H
#include <stdio.h>
#include "stdint.h"
#include "wmacros.h"
#include "wtype_h.h"

enum wstr_alloc_type {
	wstr_dynamic = 1,
	wstr_static = 2,
	wstr_gc = 3
};

def_struct( wstr ) {
	wtype * type;
	const char * text;
	size_t length;
	enum wstr_alloc_type alloc_type;
};

extern wtype wstr_type;

#define WSTR_LIT( literal ) \
	(wstr) { &wstr_type, literal, sizeof( literal ), wstr_static }


wstr wstr_from_literal( char * literal );

wstr wstr_from_dynamic( char * dynamic );

wstr wstr_from_filename( wstr filename );

wstr wstr_from_file( FILE * file );

int wstr_compare( wstr a, wstr b);


#endif /* end of include guard: WSTR_H */
