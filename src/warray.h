#include <warray_wval.h>
#include <wmacros.h>

#ifndef elem_t
  #error Must define elem_t.
  #endif

#ifndef warray_name
  #define warray_name elem_t
  #endif

#define warray_elem_wtype join_token ( elem_t, _type )

#define method( mname ) join_token ( join_token ( warray_, warray_name ), join_token ( _, mname ) )

#define warray_struct join_token ( warray_, warray_name ) 
#define warray_iter_struct join_token ( join_token ( warray_, warray_name ), _iter )

def_struct ( warray_struct ) {
  warray array;
  };

def_struct ( warray_iter_struct ) {
  warray_iter iter;
  };

bool method( init ) ( warray_struct * self, wcall error );
bool method( init_to_size ) ( warray_struct * self, size_t num_elems, wcall error );

warray_struct * method ( new ) ( wcall error );
warray_struct * method ( new_to_size ) ( size_t num_elems, wcall error );

void method ( deinit ) ( warray_struct * self );
void method ( delete ) ( warray_struct * self );


bool method ( empty ) ( warray_struct * self );
bool method ( full ) ( warray_struct * self );

void method ( push_back ) ( warray_struct * self, elem_t elem, wcall error );
void method ( push_front ) ( warray_struct * self, elem_t elem, wcall error );

elem_t method ( pop_back ) ( warray_struct * self, wcall error );
elem_t method ( pop_front ) ( warray_struct * self, wcall error );

size_t method ( length ) ( warray_struct * self );
bool method ( good_index ) ( warray_struct * self, size_t index );
elem_t * method ( index ) ( warray_struct * self, size_t index );
elem_t method ( get ) ( warray_struct * self, size_t index );
elem_t method ( set ) ( warray_struct * self, size_t index, elem_t val );

warray_iter_struct method ( start ) ( warray_struct * parent );
warray_iter_struct method ( end ) ( warray_struct * parent );

bool method ( prev ) ( warray_iter_struct * self );
bool method ( next ) ( warray_iter_struct * self );

bool method ( good ) ( warray_iter_struct * self );
elem_t method ( deref ) ( warray_iter_struct * self );


long method ( index_of ) ( warray_struct * self, elem_t to_find );
void method ( debug_print ) ( warray_struct * self );

#undef elem_t
#undef method
#undef warray_iter_struct
#undef warray_name
#undef warray_struct
