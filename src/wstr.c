#include "wstr.h"

wstr wstr_from_literal( char * literal ) {
	wstr out;
	out.text = literal;
	out.alloc_type = wstr_static;
	return out;
}

wstr wstr_from_dynamic( char * dynamic ) {
	wstr out;
	out.text = dynamic;
	out.alloc_type = wstr_dynamic;
	return out;
}
