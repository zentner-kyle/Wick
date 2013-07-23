#include <warray_wval.h>
#include <wmacros.h>

#ifndef warray_elem_t
  #error Must define warray_elem_t.
  #endif

#ifndef warray_elem_kind
  #error Please define warray_elem_kind
  #endif

#ifndef warray_elem_name
  #define warray_elem_name warray_elem_t
  #endif

#ifndef warray_name
  #define warray_name warray_elem_t
  #endif


#define warray_elem_wtype ((wtype *) & w_type_to_wtype ( warray_elem_name ))

#define method( mname ) join_token ( join_token ( warray_, warray_name ), join_token ( _, mname ) )

#define warray_struct join_token ( warray_, warray_name ) 
#define warray_iter_struct join_token ( join_token ( warray_, warray_name ), _iter )

#define pointer 0xefaced
#if warray_elem_kind == pointer
  #undef pointer
  #define warray_cast_elem( elem ) ((wval) { .warray_elem_kind = ( wobj * ) elem })
#else
  #undef pointer
  #define warray_cast_elem( elem ) ((wval) { .warray_elem_kind = elem })
  #endif

#define warray_back_cast( val ) ( ( warray_elem_t ) ( val ).warray_elem_kind )

def_struct ( warray_struct ) {
  warray array;
  };

def_struct ( warray_iter_struct ) {
  warray_iter iter;
  };
static werror * method( init ) ( warray_struct * self ) {
  return warray_init ( &self->array, warray_elem_wtype );
  }

static werror * method( init_to_size ) ( warray_struct * self, long num_elems ) {
  return warray_init_to_size ( &self->array, warray_elem_wtype, num_elems );
  }


static warray_struct * method ( new ) ( void ) {
  return ( warray_struct * ) warray_new ( warray_elem_wtype );
  }

static warray_struct * method ( new_to_size ) ( long num_elems ) {
  return ( warray_struct * ) warray_new_to_size ( warray_elem_wtype, num_elems );
  }


static void method ( deinit ) ( warray_struct * self ) {
  warray_deinit ( &self->array );
  }

static void method ( delete ) ( warray_struct * self ) {
  warray_delete ( &self->array );
  }


static bool method ( empty ) ( warray_struct * self ) {
  return warray_empty ( &self->array );
  }

static bool method ( full ) ( warray_struct * self ) {
  return warray_full ( &self->array );
  }


static werror * method ( push_back ) ( warray_struct * self, warray_elem_t elem ) {
  return warray_push_back ( &self->array, warray_cast_elem ( elem ) );
  }

static werror * method ( push_front ) ( warray_struct * self, warray_elem_t elem ) {
  return warray_push_front ( &self->array, warray_cast_elem ( elem ) );
  }


static werror * method ( pop_back ) ( warray_struct * self, warray_elem_t * elem ) {
  wval elem_val;
  werror * error = warray_pop_back ( &self->array, &elem_val );
  if ( error == w_ok ) {
    *elem = warray_back_cast ( elem_val );
    }
  return error;
  }

static werror * method ( pop_front ) ( warray_struct * self, warray_elem_t * elem ) {
  wval elem_val;
  werror * error = warray_pop_front ( &self->array, &elem_val );
  if ( error == w_ok ) {
    *elem = warray_back_cast ( elem_val );
    }
  return error;
  }


static long method ( length ) ( warray_struct * self ) {
  return warray_length ( &self->array );
  }

static bool method ( good_index ) ( warray_struct * self, long index ) {
  return warray_good_index ( &self->array, index );
  }

static werror * method ( index ) (
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

static werror * method ( remove ) ( warray_struct * self, long index ) {
  return warray_remove ( &self->array, index );
  }


static werror * method ( get ) ( warray_struct * self, long index, warray_elem_t * elem ) {
  wval elem_val;
  werror * error = warray_get ( &self->array, index, &elem_val );
  if ( error == w_ok ) {
    *elem = warray_back_cast ( elem_val );
    }
  return error;
  }

static werror * method ( set ) ( warray_struct * self, long index, warray_elem_t val ) {
  return warray_set ( &self->array, index, warray_cast_elem ( val ) );
  }


static warray_iter_struct method ( start ) ( warray_struct * parent ) {
  return (warray_iter_struct) { warray_start ( &parent->array ) };
  }

static warray_iter_struct method ( end ) ( warray_struct * parent ) {
  return (warray_iter_struct) { warray_end ( &parent->array ) };
  }


static void method ( prev ) ( warray_iter_struct * self ) {
  warray_prev ( &self->iter );
  }

static void method ( next ) ( warray_iter_struct * self ) {
  warray_next ( &self->iter );
  }


static bool method ( good ) ( warray_iter_struct * self ) {
  return warray_good ( &self->iter );
  }

static warray_elem_t method ( deref ) ( warray_iter_struct * self ) {
  return warray_back_cast ( warray_deref ( &self->iter ) );
  }

static warray_elem_t * method ( at ) ( warray_iter_struct * self, long rel_idx ) {
  return ( warray_elem_t * )  warray_at ( &self->iter, rel_idx );
  }

static bool method ( good_at ) ( warray_iter_struct * self, long rel_idx ) {
  return warray_good_at ( &self->iter, rel_idx );
  }

static werror * method ( remove_at ) ( warray_iter_struct * self, long rel_idx ) {
  return warray_remove_at ( &self->iter, rel_idx );
  }




static long method ( index_of ) ( warray_struct * self, warray_elem_t to_find ) {
  return warray_index_of ( &self->array, warray_cast_elem ( to_find ) );
  }

static void method ( debug_print ) ( warray_struct * self ) {
  warray_debug_print ( &self->array );
  }

#undef method
#undef warray_back_cast
#undef warray_cast_elem
#undef warray_elem_kind
#undef warray_elem_name
#undef warray_elem_t
#undef warray_elem_wtype
#undef warray_iter_struct
#undef warray_name
#undef warray_struct
