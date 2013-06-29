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

def_struct ( warray_struct ) {
  warray array;
  };

def_struct ( warray_iter_struct ) {
  warray_iter iter;
  };

bool method( init ) ( warray_struct * self, wcall * error );
bool method( init_to_size ) ( warray_struct * self, size_t num_elems, wcall * error );

warray_struct * method ( new ) ( wcall * error );
warray_struct * method ( new_to_size ) ( size_t num_elems, wcall * error );

void method ( deinit ) ( warray_struct * self );
void method ( delete ) ( warray_struct * self );


bool method ( empty ) ( warray_struct * self );
bool method ( full ) ( warray_struct * self );

void method ( push_back ) ( warray_struct * self, warray_elem_t elem, wcall * error );
void method ( push_front ) ( warray_struct * self, warray_elem_t elem, wcall * error );

warray_elem_t method ( pop_back ) ( warray_struct * self, wcall * error );
warray_elem_t method ( pop_front ) ( warray_struct * self, wcall * error );

size_t method ( length ) ( warray_struct * self );
bool method ( good_index ) ( warray_struct * self, size_t index );
warray_elem_t * method ( index ) ( warray_struct * self, size_t index );
warray_elem_t method ( get ) ( warray_struct * self, size_t index );
warray_elem_t method ( set ) ( warray_struct * self, size_t index, warray_elem_t val );

warray_iter_struct method ( start ) ( warray_struct * parent );
warray_iter_struct method ( end ) ( warray_struct * parent );

bool method ( prev ) ( warray_iter_struct * self );
bool method ( next ) ( warray_iter_struct * self );

bool method ( good ) ( warray_iter_struct * self );
warray_elem_t method ( deref ) ( warray_iter_struct * self );


long method ( index_of ) ( warray_struct * self, warray_elem_t to_find );
void method ( debug_print ) ( warray_struct * self );


#ifndef warray_source
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

  #endif
