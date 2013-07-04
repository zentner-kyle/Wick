#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <wtype.h>
#include <wbits.h>
#include <walloc.h>
#include <wmacros.h>

#include <warray_wval.h>

wdefine_base ( warray );

const static long warray_default_size = 8;

bool warray_init ( warray * self, wtype * warray_elem_type, wcall * error ) {
  return warray_init_to_size ( self, warray_elem_type, warray_default_size, error );
  }

bool warray_init_to_size (
    warray * self,
    wtype * warray_elem_type,
    long num_elements,
    wcall * error
    ) {
  self->warray_elem_type = warray_elem_type;
  self->space = num_elements;
  self->start = 0;
  self->past_end = 0;

  self->data = malloc ( sizeof ( wval ) * self->space );

  if ( ! self->data ) {
    winvoke ( error );
    return false;
    }
  else {
    return true;
    }
  }


warray * warray_new ( wtype * warray_elem_type, wcall * error ) {
  return warray_new_to_size ( warray_elem_type, warray_default_size, error );
  }

warray * warray_new_to_size ( wtype * warray_elem_type, long num_elements, wcall * error ) {
  warray * self = ( warray * ) malloc ( sizeof ( warray ) );
  if ( ! self ) {
    winvoke ( error );
    return NULL;
    }
  if ( ! warray_init_to_size ( self, warray_elem_type, num_elements, &null_wcall ) ) {
      free ( self );
      self = NULL;
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
  return self->start >= self->past_end;
  }


bool warray_grow ( warray * self, wcall * error ) {
  long new_size = round_up_power_2 ( self->space + 1 );
  wval * new_data = walloc_simple ( wval, new_size );
  if ( ! new_data ) {
    winvoke ( error );
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

void warray_push_back ( warray * self, wval elem, wcall * error ) {
  if ( warray_full ( self ) ) {
    if ( ! warray_grow ( self, error ) ) {
      return;
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
  }

void warray_push_front ( warray * self, wval elem, wcall * error ) {
  if ( warray_full ( self ) ) {
    warray_grow ( self, error );
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
  }

wval warray_pop_back ( warray * self, wcall * error ) {
  if ( warray_empty ( self ) ) {
    winvoke ( error );
    }

  wval to_return = self->data[--self->past_end];

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
  return to_return;
  }

wval warray_pop_front ( warray * self, wcall * error ) {
  if ( warray_empty ( self ) ) {
    winvoke ( error );
    }

  wval to_return = self->data[self->start++];

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
  return to_return;
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

wval * warray_index ( warray * self, long index ) {
  if ( index < 0 ) {
    index = warray_length ( self ) + index;
    }
  if ( warray_looped ( self ) ) {
    long end_size = self->space - self->start;
    if ( index < end_size ) {
      /* Index is in the loop-back end segment. */
      return self->data + self->start + index;
      }
    else if ( index - end_size < self->past_end ) {
      /* Index is in the normal segment. */
      return self->data + (index - end_size );
      }
    else {
      /* Index is out of bounds ( between the segments ). */
      return NULL;
      }
    }
  else {
    if ( index + self->start < self->past_end ) {
      /* Index is in bounds. */
      return self->data + self->start + index;
      }
    else {
      /* Index is out of bounds. */
      return NULL;
      }
    }
  }

wval warray_get ( warray * self, long index ) {
  return *warray_index ( self, index );
  }

wval warray_set ( warray * self, long index, wval val ) {
  wval * to_set = warray_index ( self, index );
  if ( to_set != NULL ) {
    *to_set = val;
    }
  return val;
  }


warray_iter warray_start ( warray * parent ) {
  return ( warray_iter ) {.parent = parent, .index = 0  };
  }

warray_iter warray_end ( warray * parent ) {
  return ( warray_iter ) {.parent = parent, .index = -1  };
  }


void warray_next ( warray_iter * self ) {
  self->index += 1;
  /*if ( warray_good_index ( self->parent, self->index + 1 ) ) {*/
    /*return true;*/
    /*}*/
  /*else {*/
    /*return false;*/
    /*}*/
  }

void warray_prev ( warray_iter * self ) {
  self->index -= 1;
  /*if ( warray_good_index ( self->parent, self->index - 1 ) ) {*/
    /*return true;*/
    /*}*/
  /*else {*/
    /*return false;*/
    /*}*/
  }


bool warray_good ( warray_iter * self ) {
  return warray_good_index ( self->parent, self->index );
  }

wval warray_deref ( warray_iter * self ) {
  return warray_get ( self->parent, self->index );
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
