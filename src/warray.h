#ifndef WARRAY_H
#define WARRAY_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include <wtype_h.h>
#include <wcall.h>
/* #include <walloc.h> */
/* #include <wmacros.h> */

/*
 * A looping, growing array.
 * All internal indices are in bytes.
 */

def_struct ( warray ) {
  wtype * type;
  wtype * elem_type;
  size_t space;
  size_t past_end;
  size_t start;
  void * data;
  };

def_struct ( warrayi ) {
  wtype * type;
  warray * parent;
  int index;
  };

bool warray_init_to_size ( warray * self, wtype * elem_type, size_t num_elements, wcall error );
bool warray_init ( warray * self, wtype * elem_type, wcall error );

warray * warray_new_to_size ( wtype * elem_type, size_t num_elements, wcall error );
warray * warray_new ( wtype * elem_type, wcall error );

void warray_deinit ( warray * self );
void warray_delete ( warray * self );

bool warray_grow ( warray * self, wcall error );
bool warray_empty ( warray * self );
bool warray_full ( warray * self );
void warray_push_back ( warray * self, void * elem, wcall error );
void warray_push_front ( warray * self, void * elem, wcall error );
void warray_pop_back ( warray * self, void * to_store, wcall error );
void warray_pop_front ( warray * self, void * to_store, wcall error );
bool warray_valid_index ( warray * self, size_t index );

bool warray_check_type ( warray * self, size_t type_size );

#define warrayi_deref( self, type ) \
  ( * ( type * ) ( warray_deref_ptr_check ( self, sizeof ( type ) ) ) )

void * warrayi_deref_ptr ( warrayi * self );

bool warrayi_prev ( warrayi * self );
bool warrayi_next ( warrayi * self );
warrayi warray_start ( warray * parent );
warrayi warray_end ( warray * parent );
size_t warray_length ( warray * self );
bool warray_has_index ( warray * self, size_t index );
void warray_write_default ( warray * self, size_t index, void * to_store, void * _default );
void * warray_get_default ( warray * self, size_t index, void * _default );
void warray_write ( warray * self, size_t index, void * to_store );

#define warray_get( self, index, type ) \
  ( * ( type * ) ( warray_check_type ( self, sizeof ( type ) ) ? warray_get_ptr ( self, index ) : NULL ) );

void * warray_get_ptr ( warray * self, size_t index );
void * warray_find ( warray * self, void * data );

void warray_debug_print ( warray * self );

#endif /* end of include guard: WARRAY_H */
