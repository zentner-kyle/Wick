#ifndef WALLOC_H
#define WALLOC_H
#include "wtype_h.h"
#include "werror.h"
#include "wplace.h"

#ifndef wick_base_alloc
	#define wick_base_alloc( num_bytes ) malloc( num_bytes )
#endif

#define walloc_simple( type, count ) ((type *) wick_base_alloc( sizeof( type ) * count ))

void * walloc_static( wtype * t, wcall on_error );

#endif /* end of include guard: WALLOC_H */
