
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

struct array {
	array_t * data;
	size_t fill;
	/* The last valid index by number of elements.
	 * When the array is empty, points to array_empty_index.
	 */
	size_t space; /* Records allocated space in number of elements. */
	#ifdef array_loop
		size_t start;
		/* Also points to a valid index, the first.
		 * Could point anywhere when the array is empty.
		 */
		#define array_start( self ) self.start
	#else
		#define array_start( self ) 0
	#endif
};

typedef struct array array;

struct array_iter {
	array * parent;
	size_t index; /* From start. */
};

typedef struct array_iter array_iter;

#ifdef array_last_val

array_t * p_method( va_alloc )( size_t accum, va_list lst, size_t * space, size_t * fill ) {
	array_t elem = va_arg( lst, array_t );
	array_t * data;
	size_t s;
	if ( array_equal( elem, array_last_val ) ) {
		if ( accum == 0 ) {
			data = alloc( array_t, array_default_size );
			if (data) {
				*fill = array_empty_fill;
				*space = array_default_size;
			}
		} else {
			s = round_up_power_2( accum );
			data = alloc( array_t, s );
			if ( data ) {
				*space = s;
				*fill = accum - 1;
			}
		}
		return data;
	}
	data = p_method( va_alloc )( accum + 1, lst, space, fill );
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
		t = p_method( va_alloc )( 1, lst, &n->space, &n->fill );
		if ( t ) {
			t[0] = first;
			n->data = t;
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
		t = p_method( va_alloc )( 1, lst, &self->space, &self->fill );
		if ( t ) {
			t[0] = first;
			self->data = t;
		}
	}
	va_end( lst );
}

#endif

array * method( new_empty )( context_arg ) {
	array * n = alloc( array, 1 );
	array_t * t = alloc( array_t, array_default_size );
	if ( ! n || ! t ) {
		free( n );
		free( t );
		array_alloc_error( context );
		return NULL;
	}
	n->data = t;
	n->space = array_default_size;
	n->fill = array_empty_fill;
	return n;
}

void method( init )( array * self context_arg ) {
	array_t * a = alloc( array_t, array_default_size );
	if ( ! a ) {
		array_alloc_error( context );
	} else {
		self->space = array_default_size;
		self->fill = array_empty_fill;
		#ifdef array_loop
			self->start = 0;
		#endif
	}
}

void method( deinit )( array * self ) {
	if ( self->data ) {
		free(self->data);
	}
}

void method( delete )( array * self ) {
	method( deinit )( self );
	free( self );
}

#define copy_subarray( dst, src, space ) (memcpy( (void *) (dst), (void *) (src), space * sizeof( array_t ) ))

#define move_subarray( dst, src, space ) (memmove( (void *) (dst), (void *) (src), space * sizeof( array_t ) ))

bool method( grow )( array * self context_arg ) {
	size_t new_size = round_up_power_2( self->space + 1 );
	array_t * a = alloc( array_t, new_size );
	if ( ! a ) {
		array_alloc_error( context );
		return false;
	}
	#ifdef array_loop
		if ( array_looped( self) ) {
			size_t end_size = self->space - self->start;
			copy_subarray( a, self->data + self->start, end_size );
			copy_subarray( a + self->start, self->data, self->fill );
			self->fill = end_size + self->fill;
		} else {
			copy_subarray( a, self->data + self->start, self->fill - self->start );
			self->fill = self->fill - self->start;
		}
		self->start = 0;
	#else
		copy_subarray( a, self->data, self->fill );
	#endif
	free( self->data );
	self->data = a;
	return true;
}

bool method( full )( array * self ) {
	#ifdef array_loop
		return self->start == self->fill + 1 || self->start == 0 && self->fill + 1 == self->space;
	#else
		return self->fill == self->space;
	#endif
}

bool method( empty )( array * self ) {
	return self->fill == array_empty_fill;
}

void method( push_back )( array * self, array_t elem context_arg ) {
	if ( method( full )( self ) ) {
		method( grow )( self context_pass );
	}
	if ( ! method( full )( self ) ) {
		#ifdef array_loop
			if ( self->fill == self->space ) {
				/* The case where the start has been moved forward
				 * and the fill hits the end.
				 */
				self->fill = 1;
				self->data[1] = elem;
			} else {
				/* The fill must either before or after the end, with at least one space. */
				self->data[++self->fill] = elem;
			}
		#else
			self->data[++self->fill] = elem;
		#endif
	}
}

void method( push_front )( array * self, array_t elem context_arg ) {
	if ( method( full )( self ) ) {
		method( grow )( self context_pass );
	}
	if ( ! method( full ) ) {
		#ifdef array_loop
			if ( self->start == 0 ) {
				self->start = self->space;
				self->data[self->start] = elem;
			} else {
				self->data[--self->start] = elem;
			}
		#else
			move_subarray( self->data + 1, self->data, self->fill );
			self->data[0] = elem;
		#endif
	}
}

array_t method( pop_back )( array * self context_arg ) {
	size_t index = self->fill;

	if ( method( empty )( self ) ) {
		array_empty_error( context );
		return array_empty_val;
	}

	#ifdef array_loop
		if( self->fill == 0 && self->start ) {
			self->fill = self->space;
		} else if ( self->fill == 0 || self->fill == self->start ){
			self->fill = array_empty_fill;
		}
	#else
		if ( self->fill == 0 ) {
			self->fill = array_empty_fill;
		}
	#endif

	return self->data[index];
}

array_t method( pop_front )( array * self context_arg ) {
	size_t index = array_start( self );

	if ( method( empty )( self ) ) {
		array_empty_error( context );
		return array_empty_val;
	}

	#ifdef array_loop
		if( self->start == self->space ) {
			self->start = 0;
		} else if ( self->fill == self->start ){
			self->fill = array_empty_fill;
		}
	#else
		if ( self->fill ) {
			move_subarray( self->data, self->data + 1, self->fill );
			--self->fill;
		} else {
			self->fill = array_empty_fill;
		}
	#endif

	return self->data[index];
}

array_t method( index )( array * self, size_t index context_arg ) {
	/* Note that index is an _external_ index.
	 * From it's perspective, the array is always in normal order.
	 */
	#ifdef array_loop
		if ( self->start < self->fill ) {
			/* Array is in normal order, but may be offset. */
			if ( index + self->start < self->fill ) {
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
			} else if ( index - end_size < self->fill ) {
				/* Index is in the normal segment. */
				return self->data[index - end_size];
			} else {
				/* Index is out of bounds (between the segments). */
				array_empty_error( context );
				return array_empty_val;
			}
		}
	#else
		if ( index < self->fill ) {
			return self->data[index];
		} else {
			array_empty_error( context );
			return array_empty_val;
		}
	#endif
}

#define array_looped( self ) ((self)->start > (self)->fill)

bool p_method( valid_index )( array * self, size_t index ) {
	#ifdef array_loop
		if ( array_looped( self ) ) {
			size_t end_size = self->space - self->start;
			return index < end_size || index - end_size < self->fill;
		} else {
			return index + self->start < self->fill;
		}
	#else
		if ( index < self->fill ) {
			return true;
		} else {
			return false;
		}
	#endif
}

array_iter p_method( iterator )( array * self, size_t index context_arg ) {
	if ( ! p_method( valid_index )( self, index ) ) {
		array_empty_error( context );
	}
	return (array_iter){self, index};
}

array_iter method( start )( array * self context_arg ) {
	return p_method( iterator )( self, array_start( self ) context_pass );
}

array_iter method( end )( array * self context_arg ) {
	return p_method( iterator )( self, self->fill context_pass );
}

array_t method( deref )( array_iter self ) {
	return self.parent->data[self.index];
}

bool method( next )( array_iter * self context_arg ) {
	array * parent = self->parent;
	#ifdef array_loop
		if ( array_looped( parent ) ) {
			if ( parent->start <= self->index && self->index + 1 < parent->space ) {
				/* End segment. */
				self->index += 1;
				return true;
			} else if ( self->index == parent->space - 1 ) {
				/* Loop around. */
				self->index = 0;
				return true;
			} else if ( self->index < parent->fill ) {
				/* Beginning segment. */
				self->index += 1;
				return true;
			} else {
				return false;
			}
		} else {
			if ( self->index < parent->fill ) {
				self->index += 1;
				return true;
			} else {
				return false;
			}
		}
	#else
		if ( self->index < parent->fill ) {
			self->index += 1;
			return true;
		} else {
			return false;
		}
	#endif
}

bool method( prev )( array_iter * self context_arg ) {
	#ifdef array_loop
		array * parent = self->parent;
		if ( array_looped( parent ) ) {
			if ( 0 < self->index && self->index < parent->fill ) {
				/* Beginning segment. */
				self->index -= 1;
				return true;
			}  else if ( self->index == 0 ) {
				/* Loop back. */
				self->index = parent->space - 1;
				return true;
			} else if ( parent->start < self->index && self->index < parent->space ) {
				/* End segment. */
				self->index -= 1;
				return true;
			} else {
				return false;
			}
		}
	#else
		if ( self->index > 0 ) {
			self->index -= 1;
			return true;
		} else {
			return false;
		}
	#endif
}

/*size_t p_method( before )( array * self, size_t index ) {*/
/*#ifdef array_loop*/
	/*size_t total = 0;*/
	/*if ( array_looped( self ) ) {*/
		/*if ( index < self->fill ) {*/
			/*return index + self->space - self->start;*/
		/*} else if ( self->start < index ) {*/
			/*return index - self->start;*/
		/*}*/
	/*} else {*/
		/*return index - self->start;*/
	/*}*/
/*}*/

/*void method( insert_after )( array_iter * self, array_t elem context_arg ) {*/

/*}*/
