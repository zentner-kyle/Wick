#include <warray_internal.h>
#include <wmacros.h>

#ifndef elem_t
  #error Must define elem_t.
  #endif

#define back_cast( val ) ( ( elem_t ) ( val ).i )

#ifndef warray_name
  #define warray_name elem_t
  #endif

#define warray_elem_wtype join_token ( elem_t, _type )

#define method( mname ) join_token ( join_token ( warray_, warray_name ), join_token ( _, mname ) )

#define warray_struct join_token ( warray_, warray_name ) 
#define warray_iter_struct join_token ( join_token ( warray_, warray_name ), _iter )

/* TODO: Add (optional) pointer checking. */

def_struct ( warray_struct ) {
  warray array;
  };

def_struct ( warray_iter_struct ) {
  warray_iter iter;
  };

#define inline


inline bool method( init ) ( warray_struct * self, wcall error ) {
  return warray_init ( &self->array, warray_elem_wtype, error );
  }

inline bool method( init_to_size ) ( warray_struct * self, size_t num_elems, wcall error ) {
  return warray_init_to_size ( &self->array, warray_elem_wtype, num_elems, error );
  }


inline warray_struct * method ( new ) ( wcall error ) {
  return ( warray_struct * ) warray_new ( warray_elem_wtype, error );
  }

inline warray_struct * method ( new_to_size ) ( size_t num_elems, wcall error ) {
  return ( warray_struct * ) warray_new_to_size ( warray_elem_wtype, num_elems, error );
  }


inline void method ( deinit ) ( warray_struct * self ) {
  warray_deinit ( &self->array );
  }

inline void method ( delete ) ( warray_struct * self ) {
  warray_delete ( &self->array );
  }


inline bool method ( empty ) ( warray_struct * self ) {
  return warray_empty ( &self->array );
  }

inline bool method ( full ) ( warray_struct * self ) {
  return warray_full ( &self->array );
  }


inline void method ( push_back ) ( warray_struct * self, elem_t elem, wcall error ) {
  warray_push_back ( &self->array, (wval) { elem }, error );
  }

inline void method ( push_front ) ( warray_struct * self, elem_t elem, wcall error ) {
  warray_push_front ( &self->array, (wval) { elem }, error );
  }


inline elem_t method ( pop_back ) ( warray_struct * self, wcall error ) {
  return back_cast ( warray_pop_back ( &self->array, error ) );
  }

inline elem_t method ( pop_front ) ( warray_struct * self, wcall error ) {
  return back_cast ( warray_pop_front ( &self->array, error ) );
  }


inline size_t method ( length ) ( warray_struct * self ) {
  return warray_length ( &self->array );
  }

inline bool method ( good_index ) ( warray_struct * self, size_t index ) {
  return warray_good_index ( &self->array, index );
  }

inline elem_t * method ( index ) ( warray_struct * self, size_t index ) {
  return ( elem_t * ) warray_index ( &self->array, index );
  }

inline elem_t method ( get ) ( warray_struct * self, size_t index ) {
  return back_cast ( warray_get ( &self->array, index ) );
  }

inline elem_t method ( set ) ( warray_struct * self, size_t index, elem_t val ) {
  return back_cast ( warray_set ( &self->array, index, ( wval ) { val } ) );
  }


inline warray_iter_struct method ( start ) ( warray_struct * parent ) {
  return (warray_iter_struct) { warray_start ( &parent->array ) };
  }

inline warray_iter_struct method ( end ) ( warray_struct * parent ) {
  return (warray_iter_struct) { warray_end ( &parent->array ) };
  }


inline bool method ( prev ) ( warray_iter_struct * self ) {
  return warray_prev ( &self->iter );
  }

inline bool method ( next ) ( warray_iter_struct * self ) {
  return warray_next ( &self->iter );
  }


inline bool method ( good ) ( warray_iter_struct * self ) {
  return warray_good ( &self->iter );
  }

inline elem_t method ( deref ) ( warray_iter_struct * self ) {
  return back_cast ( warray_deref ( &self->iter ) );
  }



inline long method ( index_of ) ( warray_struct * self, elem_t to_find ) {
  return warray_index_of ( &self->array, (wval) { to_find } );
  }

inline void method ( debug_print ) ( warray_struct * self ) {
  warray_debug_print ( &self->array );
  }

#undef back_cast
