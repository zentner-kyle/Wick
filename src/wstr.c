#include "wstr.h"
#include "wtype.h"

wtype wstr_type;

wstr wstr_from_literal( char * literal ) {
	wstr out;
    out.type = &wstr_type;
	out.text = literal;
	out.alloc_type = wstr_static;
	return out;
}

wstr wstr_from_dynamic( char * dynamic ) {
	wstr out;
    out.type = &wstr_type;
	out.text = dynamic;
	out.alloc_type = wstr_dynamic;
	return out;
}
