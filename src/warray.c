#include <warray_wval.h>
#include <wmacros.h>

#define warray_source
#include <warray.h>

#define back_cast( val ) ( ( warray_elem_t ) ( val ).warray_elem_kind )

bool method( init ) ( warray_struct * self, wcall * error ) {
  return warray_init ( &self->array, warray_elem_wtype, error );
  }

bool method( init_to_size ) ( warray_struct * self, long num_elems, wcall * error ) {
  return warray_init_to_size ( &self->array, warray_elem_wtype, num_elems, error );
  }


warray_struct * method ( new ) ( wcall * error ) {
  return ( warray_struct * ) warray_new ( warray_elem_wtype, error );
  }

warray_struct * method ( new_to_size ) ( long num_elems, wcall * error ) {
  return ( warray_struct * ) warray_new_to_size ( warray_elem_wtype, num_elems, error );
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


void method ( push_back ) ( warray_struct * self, warray_elem_t elem, wcall * error ) {
  warray_push_back ( &self->array, warray_cast_elem ( elem ), error );
  }

void method ( push_front ) ( warray_struct * self, warray_elem_t elem, wcall * error ) {
  warray_push_front ( &self->array, warray_cast_elem ( elem ), error );
  }


warray_elem_t method ( pop_back ) ( warray_struct * self, wcall * error ) {
  return back_cast ( warray_pop_back ( &self->array, error ) );
  }

warray_elem_t method ( pop_front ) ( warray_struct * self, wcall * error ) {
  return back_cast ( warray_pop_front ( &self->array, error ) );
  }


long method ( length ) ( warray_struct * self ) {
  return warray_length ( &self->array );
  }

bool method ( good_index ) ( warray_struct * self, long index ) {
  return warray_good_index ( &self->array, index );
  }

warray_elem_t * method ( index ) ( warray_struct * self, long index ) {
  return ( warray_elem_t * ) warray_index ( &self->array, index );
  }

void method ( remove ) ( warray_struct * self, long index ) {
  warray_remove ( &self->array, index );
  }


warray_elem_t method ( get ) ( warray_struct * self, long index ) {
  return back_cast ( warray_get ( &self->array, index ) );
  }

warray_elem_t method ( set ) ( warray_struct * self, long index, warray_elem_t val ) {
  return back_cast ( warray_set ( &self->array, index, warray_cast_elem ( val ) ) );
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

void method ( remove_at ) ( warray_iter_struct * self, long rel_idx ) {
  warray_remove_at ( &self->iter, rel_idx );
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
