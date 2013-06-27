#include <warray_wval.h>
#include <wmacros.h>

#define warray_source
#include <warray.h>

#define back_cast( val ) ( ( elem_t ) ( val ).integer )

bool method( init ) ( warray_struct * self, wcall * error ) {
  return warray_init ( &self->array, warray_elem_wtype, error );
  }

bool method( init_to_size ) ( warray_struct * self, size_t num_elems, wcall * error ) {
  return warray_init_to_size ( &self->array, warray_elem_wtype, num_elems, error );
  }


warray_struct * method ( new ) ( wcall * error ) {
  return ( warray_struct * ) warray_new ( warray_elem_wtype, error );
  }

warray_struct * method ( new_to_size ) ( size_t num_elems, wcall * error ) {
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


void method ( push_back ) ( warray_struct * self, elem_t elem, wcall * error ) {
  warray_push_back ( &self->array, (wval) { elem }, error );
  }

void method ( push_front ) ( warray_struct * self, elem_t elem, wcall * error ) {
  warray_push_front ( &self->array, (wval) { elem }, error );
  }


elem_t method ( pop_back ) ( warray_struct * self, wcall * error ) {
  return back_cast ( warray_pop_back ( &self->array, error ) );
  }

elem_t method ( pop_front ) ( warray_struct * self, wcall * error ) {
  return back_cast ( warray_pop_front ( &self->array, error ) );
  }


size_t method ( length ) ( warray_struct * self ) {
  return warray_length ( &self->array );
  }

bool method ( good_index ) ( warray_struct * self, size_t index ) {
  return warray_good_index ( &self->array, index );
  }

elem_t * method ( index ) ( warray_struct * self, size_t index ) {
  return ( elem_t * ) warray_index ( &self->array, index );
  }

elem_t method ( get ) ( warray_struct * self, size_t index ) {
  return back_cast ( warray_get ( &self->array, index ) );
  }

elem_t method ( set ) ( warray_struct * self, size_t index, elem_t val ) {
  return back_cast ( warray_set ( &self->array, index, ( wval ) { val } ) );
  }


warray_iter_struct method ( start ) ( warray_struct * parent ) {
  return (warray_iter_struct) { warray_start ( &parent->array ) };
  }

warray_iter_struct method ( end ) ( warray_struct * parent ) {
  return (warray_iter_struct) { warray_end ( &parent->array ) };
  }


bool method ( prev ) ( warray_iter_struct * self ) {
  return warray_prev ( &self->iter );
  }

bool method ( next ) ( warray_iter_struct * self ) {
  return warray_next ( &self->iter );
  }


bool method ( good ) ( warray_iter_struct * self ) {
  return warray_good ( &self->iter );
  }

elem_t method ( deref ) ( warray_iter_struct * self ) {
  return back_cast ( warray_deref ( &self->iter ) );
  }



long method ( index_of ) ( warray_struct * self, elem_t to_find ) {
  return warray_index_of ( &self->array, (wval) { to_find } );
  }

void method ( debug_print ) ( warray_struct * self ) {
  warray_debug_print ( &self->array );
  }

#undef back_cast
#undef elem_t
#undef method
#undef warray_iter_struct
#undef warray_name
#undef warray_struct
