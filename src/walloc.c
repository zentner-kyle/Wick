#include "wtype.h"
#include "werror.h"
#include "walloc.h"
#include "stdlib.h"
#include "string.h"

	/* For use in low level code. */

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
