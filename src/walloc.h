#ifndef WALLOC_H
#define WALLOC_H
#include <wtype_h.h>
#include <werror.h>

#define wick_base_alloc( num_bytes ) malloc ( num_bytes )
#define wick_base_free( ptr ) free ( ptr )
#define wick_base_memalign( num_bytes, alignment ) wick_memalign ( num_bytes, alignment )

void * wick_memalign ( size_t num_bytes, size_t alignment );

#define WICK_ARENA_ALIGNMENT ( ( size_t ) ( 1 << 12 ) )

#define walloc_simple( type, count ) ( ( type * ) wick_base_alloc ( sizeof ( type ) * count ) )

void * walloc_static ( wtype * t, wcall * on_error );

extern werror wick_out_of_memory;

#endif /* end of include guard: WALLOC_H */
