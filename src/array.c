#include "array.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

const static size_t array_default_size = 8;

static inline void * idx( void * a, size_t b ) {
	return (void *) ((uint8_t *) a + b);
}

static inline void set( void * dst, void * src, array * self ) {
	if ( dst ) {
		memcpy( dst, src, self->elem_size );
	}
}

bool array_init_to_size( array * self, size_t elem_size, size_t num_elements, delegate error ) {
	self->elem_size = elem_size;
	self->space = elem_size * num_elements;
	self->start = 0;
	self->past_end = 0;

	self->data = malloc( self->space );

	if ( ! self->data ) {
		invoke(error);
		return false;
	} else {
		return true;
	}
}

bool array_init( array * self, size_t elem_size, delegate error ) {
	return array_init_to_size( self, elem_size, array_default_size, error );
}

array * array_new_to_size( size_t elem_size, size_t num_elements, delegate error ) {
	array * self = alloc( array, 1 );
	if ( ! self ) {
		return NULL;
	}
	if ( ! array_init_to_size( self, elem_size, num_elements, null_delegate ) ) {
			free(self);
			invoke(error);
	}
	return self;
}

array * array_new( size_t elem_size, delegate error ) {
	return array_new_to_size( elem_size, array_default_size, error );
}

void array_deinit( array * self ) {
	free( self->data );
}

void array_delete( array * self ) {
	array_deinit( self );
	free( self );
}

static bool array_looped( array * self ) {
	return self->start >= self->past_end;
}

bool array_grow( array * self, delegate error ) {
	size_t new_size = round_up_power_2( self->space + 1 );
	void * new_data = malloc( self->elem_size * new_size );
	if ( ! new_data ) {
		invoke( error );
		return false;
	}
	if ( array_looped( self ) ) {
		size_t end_size = self->space - self->start;
		memcpy( new_data, idx(self->data, self->start), end_size );
		memcpy( idx(new_data, self->start), self->data, self->past_end * self->elem_size );
		self->past_end = end_size + self->past_end;
	} else {
		memcpy( new_data, idx(self->data, self->start), self->past_end - self->start );
	}
	self->start = 0;
	free( self->data );
	self->data = new_data;
	return true;
}

bool array_full( array * self ) {
	return (self->past_end != 0 && self->start == self->past_end) || (self->start == 0 && self->past_end == self->space);
}

bool array_empty( array * self ) {
	return self->past_end == 0;
}

void array_push_back( array * self, void * elem, delegate error ) {
	if ( array_full( self ) ) {
		array_grow( self, error );
	}
	if ( ! array_full( self ) ) {
		if ( self->past_end == self->space ) {
			/* Handle past_end rolling forwards. */
			self->past_end = self->elem_size;
			set( self->data, elem, self );
		} else {
			set( idx(self->data, self->past_end), elem, self );
			self->past_end += self->elem_size;
		}
	}
}

void array_push_front( array * self, void * elem, delegate error ) {
	if ( array_full( self ) ) {
		array_grow( self, error );
	}
	if ( ! array_full( self ) ) {
		 
		if ( array_empty( self ) ) {
			/* Handle empty arrays. */
			self->past_end = self->elem_size;
			set( self->data, elem, self );
		} else if ( self->start == 0 ) {
			/* Handle start rolling backwards. */
			self->start = self->space - self->elem_size;
			set( idx( self->data, self->start ), elem, self );
		} else {
			self->start -= self->elem_size;
			set( idx(self->data, self->start), elem, self );
		}
	}
}

void array_pop_back( array * self, void * to_store, delegate error ) {
	if ( array_empty( self ) ) {
		invoke( error );
	}

	set( to_store, idx( self->data, self->past_end - self->elem_size ), self );

	if ( self->past_end == self->elem_size && self->start != 0 ) {
		/* Handle past_end rolling backwards. */
		/* Implies the array is not becoming empty. */
		self->past_end = self->space;
	} else {
		/* Handles the normal case and the array becoming empty. */
		self->past_end -= self->elem_size;
		if ( self->past_end == self->start ) {
			self->past_end = 0;
		}
	}
}

void array_pop_front( array * self, void * to_store, delegate error ) {
	if ( array_empty( self ) ) {
		invoke( error );
		printf( "here\n" );
	}

	set( to_store, idx( self->data, self->start ), self );

	if ( self->start == self->space - self->elem_size && self->past_end != self->space ) {
		/* Handle start rolling forwards. */
		/* Implies the array is not becoming empty. */
		self->start = 0;
	} else {
		self->start += self->elem_size;
		if ( self->start == self->past_end ) {
			self->past_end = 0;
		}
	}
}

void * array_get_ptr( array * self, size_t index ) {
	size_t b_index = index * self->elem_size;
	if ( array_looped( self ) ) {
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

void array_write( array * self, size_t index, void * to_store ) {
	void * gotten = array_get_ptr( self, index );
	if ( gotten != NULL ) {
		set( to_store, gotten, self );
	}
}


void * array_get_default( array * self, size_t index, void * _default ) {
	void * gotten = array_get_ptr( self, index );
	if ( gotten != NULL ) {
		return gotten;
	} else {
		return _default;
	}
}

void array_write_default( array * self, size_t index, void * to_store, void * _default ) {
	void * gotten = array_get_ptr( self, index );
	if ( gotten != NULL ) {
		set( to_store, gotten, self );
	} else {
		set( to_store, _default, self );
	}
}


bool array_has_index( array * self, size_t index ) {
	size_t b_index = index * self->elem_size;
	if ( array_looped( self ) ) {
		size_t end_size = self->space - self->start;
		return b_index < end_size || b_index - end_size < self->past_end;
	} else {
		return b_index + self->start <= self->past_end;
	}
}

size_t array_length( array * self ) {
	if ( array_looped( self ) ) {
		size_t end_size = self->space - self->start;
		size_t front_size = self->past_end;
		return (end_size + front_size) / self->elem_size;
	} else {
		return (self->past_end - self->start) / self->elem_size;
	}
}

arrayi array_start( array * parent ) {
	return (arrayi){.parent = parent, .index = -1};
}

arrayi array_end( array * parent ) {
	return (arrayi){.parent = parent, .index = array_length( parent) + 1};
}

bool arrayi_next( arrayi * self ) {
	if ( array_has_index( self->parent, self->index + 1 ) ) {
		self->index += 1;
		return true;
	} else {
		return false;
	}
}

bool arrayi_prev( arrayi * self ) {
	if ( array_has_index( self->parent, self->index - 1 ) ) {
		self->index -= 1;
		return true;
	} else {
		return false;
	}
}

void * arrayi_deref_ptr( arrayi * self ) {
	return array_get_ptr( self->parent, self->index );
}

void * arrayi_deref_ptr_check( arrayi * self, size_t elem_size ) {
	if ( array_check_type( self->parent, elem_size ) ) {
		return array_get_ptr( self->parent, self->index );
	} else {
		return NULL;
	}
}

void * array_find( array * self, void * data ) {
	if ( data == NULL ) {
		return NULL;
	} else {
		arrayi i = array_start( self );
		while ( arrayi_next( &i ) ) {
			if ( memcmp( arrayi_deref_ptr( &i ), data, self->elem_size ) == 0 ) {
				return arrayi_deref_ptr( &i );
			}
		}
	}
	return NULL;
}

void array_debug_print( array * self ) {
	printf( ".elem_size = %zd\n", self->elem_size );
	printf( ".space = %zd\n", self->space );
	printf( ".past_end = %zd\n", self->past_end );
	printf( ".start = %zd\n", self->start );
	printf( ".data = %p\n", self->data );
}

bool array_check_type( array * self, size_t type_size ) {
	assert( self->elem_size == type_size );
	return self->elem_size == type_size;
}
