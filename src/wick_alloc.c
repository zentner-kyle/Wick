#include <wick_alloc.h>
#include <wick_type.h>
#include <wick_place.h>
#include <wick_error.h>
/* TODO: fix error handling! */

void * wick_host_alloc ( size_t bytes ) {
  return malloc ( bytes );
  }

void * wick_malloc ( size_t bytes ) {
  return malloc ( bytes );
  }

void * wick_cmalloc ( size_t bytes ) {
  return calloc ( bytes, sizeof ( uint8_t ) );
  }

void wick_host_free ( void * mem ) {
  free ( mem );
  }

wick_dyn wick_alloc ( wick_place * place, wick_type * type ) {
  wick_dyn result;
  result.obj = wick_malloc ( type->size );
  if ( ! result.obj ) {
    wick_signal_alloc_error ( place );
    result.type = 0;
    return result;
    }
  else {
    result.type = type;
    return result;
    }
  }
