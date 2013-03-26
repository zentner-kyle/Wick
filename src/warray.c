#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "warray.h"
#include "wtype.h"
#include "wbits.h"

const static size_t warray_default_size = 8;

static inline void * idx( void * a, size_t b ) {
	return (void *) ((uint8_t *) a + b);
}

static inline size_t warray_elem_size( warray * self ) {
	return self->elem_type->size;
}

static inline void set( void * dst, void * src, warray * self ) {
	if ( dst ) {
		memcpy( dst, src, self->elem_type->size );
	}
}

bool warray_init_to_size( warray * self, wtype * elem_type, size_t num_elements, wcall error ) {
	self->elem_type = elem_type;
	self->space = elem_type->size * num_elements;
	self->start = 0;
	self->past_end = 0;

	self->data = malloc( self->space );

	if ( ! self->data ) {
		winvoke(error);
		return false;
	} else {
		return true;
	}
}

bool warray_init( warray * self, wtype * elem_type, wcall error ) {
	return warray_init_to_size( self, elem_type, warray_default_size, error );
}

warray * warray_new_to_size( wtype * elem_type, size_t num_elements, wcall error ) {
	warray * self = (warray *) malloc( sizeof( warray ) );
	if ( ! self ) {
		return NULL;
	}
	if ( ! warray_init_to_size( self, elem_type, num_elements, null_wcall ) ) {
			free(self);
			winvoke(error);
	}
	return self;
}

warray * warray_new( wtype * elem_type, wcall error ) {
	return warray_new_to_size( elem_type, warray_default_size, error );
}

void warray_deinit( warray * self ) {
	free( self->data );
}

void warray_delete( warray * self ) {
	warray_deinit( self );
	free( self );
}

static bool warray_looped( warray * self ) {
	return self->start >= self->past_end;
}

bool warray_grow( warray * self, wcall error ) {
	size_t new_size = round_up_power_2( self->space + 1 );
	void * new_data = malloc( self->elem_type->size * new_size );
	if ( ! new_data ) {
		winvoke( error );
		return false;
	}
	if ( warray_looped( self ) ) {
		size_t end_size = self->space - self->start;
		memcpy( new_data, idx(self->data, self->start), end_size );
		memcpy( idx(new_data, self->start), self->data, self->past_end * self->elem_type->size );
		self->past_end = end_size + self->past_end;
	} else {
		memcpy( new_data, idx(self->data, self->start), self->past_end - self->start );
	}
	self->start = 0;
	free( self->data );
	self->data = new_data;
	return true;
}

bool warray_full( warray * self ) {
	return (self->past_end != 0 && self->start == self->past_end) || (self->start == 0 && self->past_end == self->space);
}

bool warray_empty( warray * self ) {
	return self->past_end == 0;
}

void warray_push_back( warray * self, void * elem, wcall error ) {
	if ( warray_full( self ) ) {
		warray_grow( self, error );
	}
	if ( ! warray_full( self ) ) {
		if ( self->past_end == self->space ) {
			/* Handle past_end rolling forwards. */
			self->past_end = self->elem_type->size;
			set( self->data, elem, self );
		} else {
			set( idx(self->data, self->past_end), elem, self );
			self->past_end += self->elem_type->size;
		}
	}
}

void warray_push_front( warray * self, void * elem, wcall error ) {
	if ( warray_full( self ) ) {
		warray_grow( self, error );
	}
	if ( ! warray_full( self ) ) {
		 
		if ( warray_empty( self ) ) {
			/* Handle empty warrays. */
			self->past_end = self->elem_type->size;
			set( self->data, elem, self );
		} else if ( self->start == 0 ) {
			/* Handle start rolling backwards. */
			self->start = self->space - self->elem_type->size;
			set( idx( self->data, self->start ), elem, self );
		} else {
			self->start -= self->elem_type->size;
			set( idx(self->data, self->start), elem, self );
		}
	}
}

void warray_pop_back( warray * self, void * to_store, wcall error ) {
	if ( warray_empty( self ) ) {
		winvoke( error );
	}

	set( to_store, idx( self->data, self->past_end - self->elem_type->size ), self );

	if ( self->past_end == self->elem_type->size && self->start != 0 ) {
		/* Handle past_end rolling backwards. */
		/* Implies the warray is not becoming empty. */
		self->past_end = self->space;
	} else {
		/* Handles the normal case and the warray becoming empty. */
		self->past_end -= self->elem_type->size;
		if ( self->past_end == self->start ) {
			self->past_end = 0;
		}
	}
}

void warray_pop_front( warray * self, void * to_store, wcall error ) {
	if ( warray_empty( self ) ) {
		winvoke( error );
		printf( "here\n" );
	}

	set( to_store, idx( self->data, self->start ), self );

	if ( self->start == self->space - self->elem_type->size && self->past_end != self->space ) {
		/* Handle start rolling forwards. */
		/* Implies the warray is not becoming empty. */
		self->start = 0;
	} else {
		self->start += self->elem_type->size;
		if ( self->start == self->past_end ) {
			self->past_end = 0;
		}
	}
}

void * warray_get_ptr( warray * self, size_t index ) {
	size_t b_index = index * self->elem_type->size;
	if ( warray_looped( self ) ) {
		size_t end_size = self->space - self->start;
		if ( index < end_size ) {
			/* Index is in the loop-back end segment. */
			return idx( self->data, self->start + b_index );
		} else if ( b_index - end_size < self->past_end ) {
			/* Index is in the normal segment. */
			return idx( self->data, b_index - end_size );
		} else {
			/* Index is out of bounds (between the segments). */
			return NULL;
		}
	} else {
		if ( b_index + self->start < self->past_end ) {
			/* Index is in bounds. */
			return idx( self->data, self->start + b_index );
		} else {
			/* Index is out of bounds. */
			return NULL;
		}
	}
}

void warray_write( warray * self, size_t index, void * to_store ) {
	void * gotten = warray_get_ptr( self, index );
	if ( gotten != NULL ) {
		set( to_store, gotten, self );
	}
}


void * warray_get_default( warray * self, size_t index, void * _default ) {
	void * gotten = warray_get_ptr( self, index );
	if ( gotten != NULL ) {
		return gotten;
	} else {
		return _default;
	}
}

void warray_write_default( warray * self, size_t index, void * to_store, void * _default ) {
	void * gotten = warray_get_ptr( self, index );
	if ( gotten != NULL ) {
		set( to_store, gotten, self );
	} else {
		set( to_store, _default, self );
	}
}


bool warray_has_index( warray * self, size_t index ) {
	size_t b_index = index * self->elem_type->size;
	if ( warray_looped( self ) ) {
		size_t end_size = self->space - self->start;
		return b_index < end_size || b_index - end_size < self->past_end;
	} else {
		return b_index + self->start <= self->past_end;
	}
}

size_t warray_length( warray * self ) {
	if ( warray_looped( self ) ) {
		size_t end_size = self->space - self->start;
		size_t front_size = self->past_end;
		return (end_size + front_size) / self->elem_type->size;
	} else {
		return (self->past_end - self->start) / self->elem_type->size;
	}
}

warrayi warray_start( warray * parent ) {
	return (warrayi){.parent = parent, .index = -1};
}

warrayi warray_end( warray * parent ) {
	return (warrayi){.parent = parent, .index = warray_length( parent) + 1};
}

bool warrayi_next( warrayi * self ) {
	if ( warray_has_index( self->parent, self->index + 1 ) ) {
		self->index += 1;
		return true;
	} else {
		return false;
	}
}

bool warrayi_prev( warrayi * self ) {
	if ( warray_has_index( self->parent, self->index - 1 ) ) {
		self->index -= 1;
		return true;
	} else {
		return false;
	}
}

void * warrayi_deref_ptr( warrayi * self ) {
	return warray_get_ptr( self->parent, self->index );
}

void * warrayi_deref_ptr_check( warrayi * self, wtype * elem_type ) {
	if ( warray_check_type( self->parent, elem_type->size ) ) {
		return warray_get_ptr( self->parent, self->index );
	} else {
		return NULL;
	}
}

void * warray_find( warray * self, void * data ) {
	if ( data == NULL ) {
		return NULL;
	} else {
		warrayi i = warray_start( self );
		while ( warrayi_next( &i ) ) {
			if ( memcmp( warrayi_deref_ptr( &i ), data, self->elem_type->size ) == 0 ) {
				return warrayi_deref_ptr( &i );
			}
		}
	}
	return NULL;
}

void warray_debug_print( warray * self ) {
	printf( ".elem_type->size = %zd\n", self->elem_type->size );
	printf( ".space = %zd\n", self->space );
	printf( ".past_end = %zd\n", self->past_end );
	printf( ".start = %zd\n", self->start );
	printf( ".data = %p\n", self->data );
}

bool warray_check_type( warray * self, size_t type_size ) {
	assert( self->elem_type->size == type_size );
	return self->elem_type->size == type_size;
}
