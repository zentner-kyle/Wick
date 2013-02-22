#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "delegate.h"
#include "alloc.h"

/*
 * A looping, growing array.
 * All internal indices are in bytes.
 */

struct array {
	size_t elem_size;
	size_t space;
	size_t past_end;
	size_t start;
	void * data;
};

typedef struct array array;

struct arrayi {
	array * parent;
	int index;
};

typedef struct arrayi arrayi;


bool array_init_to_size( array * self, size_t elem_size, size_t num_elements, delegate error );
bool array_init( array * self, size_t elem_size, delegate error );

array * array_new_to_size( size_t elem_size, size_t num_elements, delegate error );
array * array_new( size_t elem_size, delegate error );

void array_deinit( array * self );
void array_delete( array * self );

bool array_grow( array * self, delegate error );
bool array_empty( array * self );
bool array_full( array * self );
void array_push_back( array * self, void * elem, delegate error );
void array_push_front( array * self, void * elem, delegate error );
void array_pop_back( array * self, void * to_store, delegate error );
void array_pop_front( array * self, void * to_store, delegate error );
bool array_valid_index( array * self, size_t index );

bool array_check_type( array * self, size_t type_size );

#define arrayi_deref( self, type ) \
	(*(type *)(array_deref_ptr_check( self, sizeof( type ) )))

void * arrayi_deref_ptr( arrayi * self );

bool arrayi_prev( arrayi * self );
bool arrayi_next( arrayi * self );
arrayi array_start( array * parent );
arrayi array_end( array * parent );
size_t array_length( array * self );
bool array_has_index( array * self, size_t index );
void array_write_default( array * self, size_t index, void * to_store, void * _default );
void * array_get_default( array * self, size_t index, void * _default );
void array_write( array * self, size_t index, void * to_store );

#define array_get( self, index, type ) \
	(*(type *)(array_check_type( self, sizeof( type ) ) ? array_get_ptr( self, index ) : NULL ));

void * array_get_ptr( array * self, size_t index );
void * array_find( array * self, void * data );

void array_debug_print( array * self );
