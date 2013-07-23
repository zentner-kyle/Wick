#ifndef WALLOC_H
#define WALLOC_H
#include <werror.h>

#define walloc_simple( type, count ) ( ( type * ) malloc ( sizeof ( type ) * count ) )

extern werror wick_out_of_memory;

#endif /* end of include guard: WALLOC_H */
