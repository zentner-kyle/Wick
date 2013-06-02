#include "wtype.h"
#include "werror.h"
#include "walloc.h"
#include "stdlib.h"
#include "string.h"
#include <assert.h>

	/* For use in low level code. */

werror wick_out_of_memory = { &werror_type, WSTR_LIT( "Out of memory" ) };

void * walloc_static( wtype * t, wcall on_error ) {
	void * result = wick_base_alloc( t->size );
	if ( result ) {
		/* Hopefully makes finding bugs easier. */
		memset( result, 0xff, t->size );
	} else {
		winvoke(on_error);
	}
	return result;
}

wobj * walloc( wplace * place, wtype * type ) {
	void * to_store = wick_base_alloc( type->size );
	if ( ! to_store ) {
		wplace_signal_error( place, &wick_out_of_memory );
		return NULL;
	} else {
		return (wobj *) to_store;
	}
}

void * wick_memalign( size_t num_bytes, size_t alignment ) {
	void * to_write;
	assert(!(alignment & (alignment - 1)));
	if( posix_memalign( &to_write, alignment, num_bytes ) ) {
		return NULL;
	} else {
		return to_write;
	}
}
