#include <warray_wval.h>
#include <wmacros.h>

#define warray_source
#include <warray.h>

#define back_cast( val ) ( ( warray_elem_t ) ( val ).warray_elem_kind )

werror * method( init ) ( warray_struct * self ) {
  return warray_init ( &self->array, warray_elem_wtype );
  }

werror * method( init_to_size ) ( warray_struct * self, long num_elems ) {
  return warray_init_to_size ( &self->array, warray_elem_wtype, num_elems );
  }


warray_struct * method ( new ) ( void ) {
  return ( warray_struct * ) warray_new ( warray_elem_wtype );
  }

warray_struct * method ( new_to_size ) ( long num_elems ) {
  return ( warray_struct * ) warray_new_to_size ( warray_elem_wtype, num_elems );
  }


void method ( deinit ) ( warray_struct * self ) {
  warray_deinit ( &self->array );
  }

void method ( delete ) ( warray_struct * self ) {
  warray_delete ( &self->array );
  }


bool method ( empty ) ( warray_struct * self ) {
  return warray_empty ( &self->array );
  }

bool method ( full ) ( warray_struct * self ) {
  return warray_full ( &self->array );
  }


werror * method ( push_back ) ( warray_struct * self, warray_elem_t elem ) {
  return warray_push_back ( &self->array, warray_cast_elem ( elem ) );
  }

werror * method ( push_front ) ( warray_struct * self, warray_elem_t elem ) {
  return warray_push_front ( &self->array, warray_cast_elem ( elem ) );
  }


werror * method ( pop_back ) ( warray_struct * self, warray_elem_t * elem ) {
  wval elem_val;
  werror * error = warray_pop_back ( &self->array, &elem_val );
  if ( error == w_ok ) {
    *elem = back_cast ( elem_val );
    }
  return error;
  }

werror * method ( pop_front ) ( warray_struct * self, warray_elem_t * elem ) {
  wval elem_val;
  werror * error = warray_pop_front ( &self->array, &elem_val );
  if ( error == w_ok ) {
    *elem = back_cast ( elem_val );
    }
  return error;
  }


long method ( length ) ( warray_struct * self ) {
  return warray_length ( &self->array );
  }

bool method ( good_index ) ( warray_struct * self, long index ) {
  return warray_good_index ( &self->array, index );
  }

werror * method ( index ) (
    warray_struct * self,
    long index,
    warray_elem_t ** elem
    ) {
  wval * elem_val_ptr;
  werror * error = warray_index ( &self->array, index, &elem_val_ptr );
  if ( error == w_ok ) {
    *elem = ( warray_elem_t * ) elem_val_ptr;
    }
  return error;
  }

werror * method ( remove ) ( warray_struct * self, long index ) {
  return warray_remove ( &self->array, index );
  }


werror * method ( get ) ( warray_struct * self, long index, warray_elem_t * elem ) {
  wval elem_val;
  werror * error = warray_get ( &self->array, index, &elem_val );
  if ( error == w_ok ) {
    *elem = back_cast ( elem_val );
    }
  return error;
  }

werror * method ( set ) ( warray_struct * self, long index, warray_elem_t val ) {
  return warray_set ( &self->array, index, warray_cast_elem ( val ) );
  }


warray_iter_struct method ( start ) ( warray_struct * parent ) {
  return (warray_iter_struct) { warray_start ( &parent->array ) };
  }

warray_iter_struct method ( end ) ( warray_struct * parent ) {
  return (warray_iter_struct) { warray_end ( &parent->array ) };
  }


void method ( prev ) ( warray_iter_struct * self ) {
  warray_prev ( &self->iter );
  }

void method ( next ) ( warray_iter_struct * self ) {
  warray_next ( &self->iter );
  }


bool method ( good ) ( warray_iter_struct * self ) {
  return warray_good ( &self->iter );
  }

warray_elem_t method ( deref ) ( warray_iter_struct * self ) {
  return back_cast ( warray_deref ( &self->iter ) );
  }

warray_elem_t * method ( at ) ( warray_iter_struct * self, long rel_idx ) {
  return ( warray_elem_t * )  warray_at ( &self->iter, rel_idx );
  }

bool method ( good_at ) ( warray_iter_struct * self, long rel_idx ) {
  return warray_good_at ( &self->iter, rel_idx );
  }

werror * method ( remove_at ) ( warray_iter_struct * self, long rel_idx ) {
  return warray_remove_at ( &self->iter, rel_idx );
  }




long method ( index_of ) ( warray_struct * self, warray_elem_t to_find ) {
  return warray_index_of ( &self->array, warray_cast_elem ( to_find ) );
  }

void method ( debug_print ) ( warray_struct * self ) {
  warray_debug_print ( &self->array );
  }

#undef back_cast
#undef warray_elem_t
#undef method
#undef warray_iter_struct
#undef warray_name
#undef warray_struct

#undef warray_elem_t
#undef method
#undef warray_elem_wtype
#undef warray_iter_struct
#undef warray_name
#undef warray_source
#undef warray_struct
#undef warray_elem_name
#undef warray_elem_kind
#undef warray_cast_elem
