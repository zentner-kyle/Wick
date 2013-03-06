#include <assert.h>
#include <stdio.h>

#ifndef array_t
	#error No type passed to def_array.c, please define array_t.
#endif

#ifndef array_default_size
	#define array_default_size 8
#endif

#ifndef array_end_arg
	#define array_end_arg ~(array_t)0
#endif

#ifndef array_cmp
	#define array_cmp( a, b ) ((a) - (b))
	#define array_equal( a, b ) ((a) == (b))
#else
	#ifndef array_equal
		#define array_equal(a, b) (array_cmp((a), (b)) == 0)
	#endif
#endif

#define array macro_join( array_t, _array )

#define array_iter macro_join( array, _iter )

#define method( name ) macro_join( array, macro_join( _, name ) )

#define p_method( name ) macro_join( macro_join( private_, array ), macro_join( _, name ) )

#define array_empty_fill (~0L)

#ifndef array_context
	#define context_arg
	#define context_pass
	#define array_alloc_error( context )
	#define array_empty_error( context )
#else
	#define context_arg , array_context context
	#define context_pass , context
	#ifndef array_alloc_error
		#error Please define array_alloc_error.
	#endif
	#ifndef array_empty_error
		#error Please define array_empty_error.
	#endif
#endif

#ifndef array_empty_val
	#define array_empty_val ((array_t)0)
#endif

/*
 * All internal indices in elements.
 */

struct array {
	array_t * data;
	size_t past_end;
	size_t space; /* Records allocated space in number of elements. */
	size_t start;
	/* Also points to a valid index, the first.
	 * Could point anywhere when the array is empty.
	 */
};

typedef struct array array;

struct array_iter {
	array * parent;
	size_t index; /* From start. */
};

typedef struct array_iter array_iter;

void method( debug_print )( array * self ) {
	printf( ".space = %zd\n", self->space );
	printf( ".past_end = %zd\n", self->past_end );
	printf( ".start = %zd\n", self->start );
	printf( ".data = %p\n", self->data );
	fflush( stdout );
}

#ifdef array_last_val

array_t * p_method( va_alloc )( size_t accum, va_list lst, size_t * space, size_t * past_end ) {
	array_t elem = va_arg( lst, array_t );
	array_t * data;
	size_t s;
	if ( array_equal( elem, array_last_val ) ) {
		if ( accum == 0 ) {
			data = alloc( array_t, array_default_size );
			if (data) {
				*past_end = 0;
				*space = array_default_size;
			}
		} else {
			s = round_up_power_2( accum );
			data = alloc( array_t, s );
			if ( data ) {
				*space = s;
				*past_end = accum;
			}
		}
		return data;
	}
	data = p_method( va_alloc )( accum + 1, lst, space, past_end );
	if ( data ) {
		data[accum] = elem;
	}
	return data;
}

array * method( new_with )( array_t first,... ) {
	va_list lst;
	array * n = alloc( array, 1 );
	array_t * t;
	va_start( lst, first );
	if ( n ) {
		t = p_method( va_alloc )( 1, lst, &n->space, &n->past_end );
		if ( t ) {
			t[0] = first;
			n->data = t;
			n->start = 0;
		} else {
			free( n );
			n = NULL;
		}
	}
	va_end( lst );
	return n;
}

void method( init_with )( array * self, array_t first, ... ) {
	va_list lst;
	array_t * t;
	va_start( lst, first );
	if ( self ) {
		t = p_method( va_alloc )( 1, lst, &self->space, &self->past_end );
		if ( t ) {
			t[0] = first;
			self->data = t;
			self->start = 0;
		}
	}
	va_end( lst );
}

#endif

void method( init_to_size )( array * self, size_t size context_arg ) {
	array_t * a = NULL;
	if ( size != 0 ) {
		a = alloc( array_t, size );
		if ( ! a ) {
			array_alloc_error( context );
			return;
		}
	}
	self->space = size;
	self->past_end = 0;
	self->start = 0;
	self->data = a;
}

void method( init )( array * self context_arg ) {
	method( init_to_size )( self, array_default_size context_pass );
}

array * method( new_empty )( context_arg ) {
	array * n = alloc( array, 1 );
	if ( n ) {
		method( init )( n );
	}
	if ( ! n || ! n->data ) {
		free( n->data );
		free( n );
		array_alloc_error( context );
		return NULL;
	}
	return n;
}

void method( deinit )( array * self ) {
	if ( self ) {
		free(self->data);
	}
}

size_t method( size )( array * self ) {
	return ( self->space - self->start ) + self->past_end;
}

void method( delete )( array * self ) {
	method( deinit )( self );
	free( self );
}

static inline bool p_method( looped )( array * self ) {
	return self->start >= self->past_end;
}

static inline void p_method( copy_subarray )( array_t * dst, array_t * src, size_t space ) {
	/*printf(" space = %zd\n", space );*/
	/*fflush( stdout );*/
	if ( space != 0) {
		memcpy( (void *) (dst), (void *) (src), space * sizeof( array_t ) );
	}
}

static inline void p_method( move_subarray )( array_t * dst, array_t * src, size_t space ) {
	if ( space != 0) {
		memmove( (void *) (dst), (void *) (src), space * sizeof( array_t ) );
	}
}

/*#define copy_subarray( dst, src, space ) (memcpy( (void *) (dst), (void *) (src), space * sizeof( array_t ) ))*/

/*#define move_subarray( dst, src, space ) (memmove( (void *) (dst), (void *) (src), space * sizeof( array_t ) ))*/

bool method( grow )( array * self context_arg ) {
	printf( "!!!    growing! \n");
	size_t new_size = round_up_power_2( self->space + 1 );
	if ( new_size < array_default_size ) {
		new_size = array_default_size;
	}
	printf( "new_size = %zd\n", new_size );
	fflush( stdout );
	array_t * new_data = alloc( array_t, new_size );
	if ( ! new_data ) {
		printf( "alloc failed\n" );
		fflush( stdout );
		array_alloc_error( context );
		return false;
	}
	if ( p_method( looped )( self ) ) {
		printf( "looped\n" );
		fflush( stdout );
		size_t end_size = self->space - self->start;
		p_method( copy_subarray )( new_data, self->data + self->start, end_size );
		p_method( copy_subarray )( new_data + self->start, self->data, self->past_end );
		self->past_end = end_size + self->past_end;
		self->start = 0;
	} else {
		printf( "not looped\n" );
		fflush( stdout );
		method( debug_print )( self );
	/*printf( "here!\n");*/
	/*fflush(stdout);*/
		p_method( copy_subarray )( new_data, self->data + self->start, self->past_end - self->start );
		self->past_end = self->past_end - self->start;
		self->start = 0;
	/*printf( "here2!\n");*/
	/*fflush(stdout);*/
	}
	self->start = 0;
	free( self->data );
	self->data = new_data;
	self->space = new_size;
	return true;
}

bool method( full )( array * self ) {
	if ( self->space == 0 ) {
		return true;
	} else if ( self->past_end == self->start ) {
		/* Looped back onto itself. */
		return true;
	} else if ( self->start == 0 && self->past_end == self->space ) {
		/* Normal full condition. */
		return true;
	}
	printf( " not full \n");
	return false;
}

bool method( empty )( array * self ) {
	return self->past_end == 0;
}

void method( push_back )( array * self, array_t elem context_arg ) {
	if ( ! method( full )( self ) || method( grow )( self ) ) {
		if ( self->past_end == self->space ) {
			/* The case where the start has been moved forward
			 * and the past_end hits the end.
			 */
			self->past_end = 1;
			self->data[0] = elem;
		} else {
			/* The past_end must either before or after the end, with at least one space. */
			self->data[self->past_end++] = elem;
		}
	}
}

void method( push_front )( array * self, array_t elem context_arg ) {
	if ( ! method( full )( self ) || method( grow )( self ) ) {
		if ( self->start == 0 ) {
			/* Loop back. */
			self->start = self->space - 1;
			self->data[self->start] = elem;
			if ( self->past_end == 0 ) {
				self->past_end = self->space;
			}
		} else {
			self->start -= 1;
			self->data[self->start] = elem;
		}
	}
}

array_t method( pop_back )( array * self context_arg ) {
	size_t index;

	if ( method( empty )( self ) ) {
		array_empty_error( context );
		return array_empty_val;
	}

	/* past_end cannot be zero, because the array is not empty. */
	self->past_end -= 1;
	index = self->past_end;
	if (self->past_end == 0) {
		/* Loop around */
		self->past_end = self->space;
	}
	return self->data[index];
}

array_t method( pop_front )( array * self context_arg ) {
	size_t index = self->start;

	if ( method( empty )( self ) ) {
		array_empty_error( context );
		return array_empty_val;
	}

	self->start += 1;

	if ( self->start == self->past_end ){
		self->start = 0;
		self->past_end = 0;
	}
	if( self->start == self->space ) {
		/* Loop forward. */
		self->start = 0;
	} 

	return self->data[index];
}

array_t method( index )( array * self, size_t index context_arg ) {
	/* Note that index is an _external_ index.
	 * From it's perspective, the array is always in normal order.
	 */
	if ( self->start < self->past_end ) {
		/* Array is in normal order, but may be offset. */
		if ( index + self->start < self->past_end ) {
			/* Index is in bounds. */
			return self->data[self->start + index];
		} else {
			/* Index is out of bounds. */
			array_empty_error( context );
			return array_empty_val;
		}
	} else {
		size_t end_size = self->space - self->start;
		/* Array is in looped order. */
		if ( index < end_size ) {
			/* Index is in the loop-back end segment. */
			return self->data[self->start + index];
		} else if ( index - end_size < self->past_end ) {
			/* Index is in the normal segment. */
			return self->data[index - end_size];
		} else {
			/* Index is out of bounds (between the segments). */
			array_empty_error( context );
			return array_empty_val;
		}
	}
}

bool p_method( valid_index )( array * self, size_t index ) {
	if ( p_method( looped )( self ) ) {
		size_t end_size = self->space - self->start;
		return index < end_size || index - end_size < self->past_end;
	} else {
		return index + self->start < self->past_end;
	}
}

array_iter p_method( iterator )( array * self, size_t index context_arg ) {
	if ( ! p_method( valid_index )( self, index ) ) {
		array_empty_error( context );
	}
	return (array_iter){self, index};
}

array_iter method( start )( array * self context_arg ) {
	return p_method( iterator )( self, self->start context_pass );
}

array_iter method( end )( array * self context_arg ) {
	return p_method( iterator )( self, self->past_end context_pass );
}

array_t method( deref )( array_iter self ) {
	return self.parent->data[self.index];
}

bool method( valid )( array_iter self context_arg ) {
	return p_method( valid_index )( self.parent, self.index );
}

array_iter method( next )( array_iter self context_arg ) {
	size_t index = self.index + 1;
	if ( p_method( looped )( self.parent ) && index == self.parent->space ) {
		index = 0;
	}
	return (array_iter){.parent = self.parent, .index = index};
}

array_iter method( prev )( array_iter self context_arg ) {
	size_t index = self.index - 1;
	if ( p_method( looped )( self.parent ) && index == 0 ) {
		index = self.parent->space - 1;
	}
	return (array_iter){.parent = self.parent, .index = index};
}
