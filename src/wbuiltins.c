#include <assert.h>
#include "stdbool.h"
#include "wbuiltins.h"

bool wbuiltins_initialized = false;

wtype_base wtype_int;

#define init_builtin(w) \
	wtype_init_base( &(wtype_##w), wstr_from_literal( #w ), sizeof( w ) )

void wbuiltins_init() {
	assert( !wbuiltins_initialized );
	init_builtin( int );
}

#undef init_builtin
