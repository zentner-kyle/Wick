#ifndef WICK_ALLOC_H
#define WICK_ALLOC_H
#include <stdint.h>
#include <stdlib.h>
#include <wick_dyn.h>

struct wick_type;
struct wick_place;

void * wick_host_alloc ( size_t bytes );

void * wick_malloc ( size_t bytes );

void * wick_cmalloc ( size_t bytes );

void wick_host_free ( void * mem );

#define WICK_ALLOC( _type_ ) ( ( _type_* )wick_malloc ( sizeof ( _type_ ) ) )

wick_dyn wick_alloc ( struct wick_place * place, struct wick_type * type );


#endif /* WICK_ALLOC_H */
