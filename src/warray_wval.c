#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <wtype.h>
#include <wbits.h>
#include <wmacros.h>
#include <werror.h>
#include <walloc.h>

#include <warray_wval.h>

wdefine_base ( warray );

const static long warray_default_size = 8;
werror warray_no_elem;

werror * warray_init ( warray * self, wtype * warray_elem_type ) {
  return warray_init_to_size ( self, warray_elem_type, warray_default_size );
  }

werror * warray_init_to_size (
    warray * self,
    wtype * warray_elem_type,
    long num_elements
    ) {
  self->warray_elem_type = warray_elem_type;
  self->space = num_elements;
  self->start = 0;
  self->past_end = 0;

  self->data = malloc ( sizeof ( wval ) * self->space );

  if ( ! self->data ) {
    return &wick_out_of_memory;
    }
  else {
    return w_ok;
    }
  }


warray * warray_new ( wtype * warray_elem_type ) {
  return warray_new_to_size ( warray_elem_type, warray_default_size );
  }

warray * warray_new_to_size ( wtype * warray_elem_type, long num_elements ) {
  warray * self = ( warray * ) malloc ( sizeof ( warray ) );
  if ( ! self ) {
    return NULL;
    }
  if ( warray_init_to_size ( self, warray_elem_type, num_elements ) != w_ok ) {
      free ( self );
      return NULL;
    }
  return self;
  }


void warray_deinit ( warray * self ) {
  free ( self->data );
  }

void warray_delete ( warray * self ) {
  warray_deinit ( self );
  free ( self );
  }


static bool warray_looped ( warray * self ) {
  return self->start > self->past_end;
  }


bool warray_grow ( warray * self ) {
  long new_size = round_up_power_2 ( self->space + 1 );
  wval * new_data = walloc_simple ( wval, new_size );
  if ( ! new_data ) {
    return false;
    }
  if ( warray_looped ( self ) ) {
    long end_size = self->space - self->start;
    memcpy ( ( void * ) new_data,
             (void *) ( self->data + self->start ),
             end_size * sizeof ( wval ) );
    memcpy ( ( void * ) ( new_data + self->start ),
             ( void * ) self->data,
             self->past_end * sizeof ( wval ) );
    self->past_end = end_size + self->past_end;
    }
  else {
    memcpy ( ( void * ) new_data,
             ( void * ) ( self->data + self->start ),
             sizeof ( wval ) * (self->past_end - self->start ) );
    }
  self->start = 0;
  free ( self->data );
  self->data = new_data;
  self->space = new_size;
  return true;
  }

bool warray_empty ( warray * self ) {
  return self->past_end == 0;
  }

bool warray_full ( warray * self ) {
  return ( self->past_end != 0 && self->start == self->past_end ) || ( self->start == 0 && self->past_end == self->space );
  }

werror * warray_push_back ( warray * self, wval elem ) {
  if ( warray_full ( self ) ) {
    if ( ! warray_grow ( self ) ) {
      return &wick_out_of_memory;
      }
    }
  if ( self->past_end == self->space ) {
    /* Handle past_end rolling forwards. */
    self->past_end = 1;
    self->data[0] = elem;
    }
  else {
    self->data[self->past_end++] = elem;
    }
  return w_ok;
  }

werror * warray_push_front ( warray * self, wval elem ) {
  if ( warray_full ( self ) ) {
    warray_grow ( self );
    }
  if ( ! warray_full ( self ) ) {
     
    if ( warray_empty ( self ) ) {
      /* Handle empty warrays. */
      self->past_end = 1;
      }
    else if ( self->start == 0 ) {
      /* Handle start rolling backwards. */
      self->start = self->space - 1;
      }
    else {
      --self->start;
      }
    self->data[self->start] = elem;
    }
  else {
    return &wick_out_of_memory;
    }
  return w_ok;
  }

werror * warray_pop_back ( warray * self, wval * elem ) {
  if ( warray_empty ( self ) ) {
    return &warray_no_elem;
    }

  *elem = self->data[--self->past_end];

  if ( self->past_end == 1 && self->start != 0 ) {
    /*
     * Handle past_end rolling backwards.
     * Implies the warray is not becoming empty.
     */
    self->past_end = self->space;
    }
  else {
    /* Handles the normal case and the warray becoming empty. */
    if ( self->past_end == self->start ) {
      /* Array is emptied. */
      self->past_end = 0;
      self->start = 0;
      }
    }
  return w_ok;
  }

werror * warray_pop_front ( warray * self, wval * elem ) {
  if ( warray_empty ( self ) ) {
    return &warray_no_elem;
    }

  *elem = self->data[self->start++];

  if ( self->start == self->space && self->past_end != self->space ) {
    /*
     * Handle start rolling forwards.
     * Implies the warray is not becoming empty.
     */
    self->start = 0;
    }
  else if ( self->start == self->past_end ) {
    /* Array is empitied. */
    self->past_end = 0;
    }
  return w_ok;
  }


long warray_length ( warray * self ) {
  if ( warray_looped ( self ) ) {
    long end_size = self->space - self->start;
    long front_size = self->past_end;
    return end_size + front_size;
    }
  else {
    return self->past_end - self->start;
    }
  }

bool warray_good_index ( warray * self, long index ) {
  return index < warray_length ( self ) && index >= -warray_length ( self );
  }

werror * warray_index ( warray * self, long index, wval ** elem ) {
  if ( index < 0 ) {
    index = warray_length ( self ) + index;
    }
  if ( warray_looped ( self ) ) {
    long end_size = self->space - self->start;
    if ( index < end_size ) {
      /* Index is in the loop-back end segment. */
      *elem = self->data + self->start + index;
      }
    else if ( index - end_size < self->past_end ) {
      /* Index is in the normal segment. */
      *elem = self->data + (index - end_size );
      }
    else {
      /* Index is out of bounds ( between the segments ). */
      return &warray_no_elem;
      }
    }
  else {
    if ( index + self->start < self->past_end ) {
      /* Index is in bounds. */
      *elem = self->data + self->start + index;
      }
    else {
      /* Index is out of bounds. */
      return &warray_no_elem;
      }
    }
  return w_ok;
  }

werror * warray_remove ( warray * self, long index ) {
  if ( index < 0 ) {
    index = warray_length ( self ) + index;
    }
  if ( warray_looped ( self ) ) {
    long end_size = self->space - self->start;
    if ( index < end_size ) {
      /* Index is in the loop-back end segment. */
      void * target = ( void * ) ( self->data + self->start + 1);
      void * source = ( void * ) ( self->data + self->start );
      memmove ( target, source, sizeof ( wval ) * index );
      self->start += 1;
      if ( self->start == self->space ) {
        self->start = 0;
        }
      }
    else if ( index - end_size < self->past_end ) {
      /* Index is in the normal segment. */
      long sindex = index - end_size;
      void * target = ( void * ) ( self->data + sindex );
      void * source = ( void * ) ( self->data + sindex + 1 );
      size_t size = self->past_end - sindex - 1;
      memmove ( target, source, sizeof ( wval ) * size );
      self->past_end -= 1;
      if ( self->past_end == 0 ) {
        self->past_end = self->space;
        }
      }
    else {
      /* Index is out of bounds ( between the segments ). */
      return &warray_no_elem;
      }
    }
  else {
    if ( index + self->start < self->past_end ) {
      /* Index is in bounds. */
      size_t sindex = self->start + index;
      void * target = ( void * ) ( self->data + sindex );
      void * source = ( void * ) ( self->data + sindex + 1 );
      size_t size = self->past_end - sindex - 1;
      memmove ( target, source, sizeof ( wval ) * size );
      self->past_end -= 1;
      if ( self->start == self->past_end ) {
        self->past_end = 0;
        self->start = 0;
        }
      }
    else {
      /* Index is out of bounds. */
      return &warray_no_elem;
      }
    }
  return w_ok;
  }

werror * warray_get ( warray * self, long index, wval * elem ) {
  wval * elem_ptr;
  werror * e = warray_index ( self, index, &elem_ptr );
  if ( e == w_ok ) {
    *elem = *elem_ptr;
    }
  return e;
  }

werror * warray_set ( warray * self, long index, wval val ) {
  wval * elem_ptr;
  werror * e = warray_index ( self, index, &elem_ptr );
  if ( e == w_ok ) {
    *elem_ptr = val;
    }
  return e;
  }


warray_iter warray_start ( warray * parent ) {
  return ( warray_iter ) {.parent = parent, .index = 0  };
  }

warray_iter warray_end ( warray * parent ) {
  return ( warray_iter ) {.parent = parent, .index = -1  };
  }


void warray_next ( warray_iter * self ) {
  self->index += 1;
  }

void warray_prev ( warray_iter * self ) {
  self->index -= 1;
  }


bool warray_good ( warray_iter * self ) {
  return warray_good_index ( self->parent, self->index );
  }

wval warray_deref ( warray_iter * self ) {
  wval elem;
  werror * e = warray_get ( self->parent, self->index, &elem );
  if (e == w_ok) {
    return elem;
    }
  else {
    assert ( false );
    }
  }

wval * warray_at ( warray_iter * self, long rel_idx ) {
  wval * elem_ptr;
  werror * e = warray_index ( self->parent, self->index + rel_idx, &elem_ptr );
  if ( e == w_ok ) {
    return elem_ptr;
    }
  else {
    return NULL;
    }
  }

bool warray_good_at ( warray_iter * self, long rel_idx ) {
  if ( self->index < 0 ) {
    return rel_idx + self->index < 0 &&
           rel_idx + self->index >= - warray_length ( self->parent );
    }
  else {
    return rel_idx + self->index >= 0 &&
           rel_idx + self->index < warray_length ( self->parent );
    }
  }

werror * warray_remove_at ( warray_iter * self, long rel_idx ) {
  return warray_remove ( self->parent, self->index + rel_idx );
  }



long warray_index_of ( warray * self, wval to_find ) {
  warray_iter i = warray_start ( self );
  while ( warray_good ( &i ) ) {
    if ( to_find.integer == warray_deref ( &i ).integer ) {
      return i.index;
      }
    warray_next ( &i );
    }
  return -1;
  }

void warray_debug_print ( warray * self ) {
  printf ( ".warray_elem_type->size = %zd\n", sizeof ( wval ) );
  printf ( ".space = %zd\n", self->space );
  printf ( ".past_end = %zd\n", self->past_end );
  printf ( ".start = %zd\n", self->start );
  printf ( ".data = %p\n", self->data );
  printf ( "data contents:\n" );
  uint32_t * data = ( uint32_t * ) self->data;
  for ( int i = 0; i < self->space; i++ ) {
    printf ( "%.8x ", data[ i ] );
    }
  printf ( "\n" );
  }
