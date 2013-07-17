#ifndef WARRAY_INTERNAL_H

#define WARRAY_INTERNAL_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include <wtype_h.h>
#include <wcall.h>
#include <wval.h>
#include <wmacros.h> 

/*
 * A looping, growing array.
 */

def_struct ( warray ) {
  wtype * type;
  wtype * warray_elem_type;
  long space;
  long past_end;
  long start;
  wval * data;
  };

def_struct ( warray_iter ) {
  wtype * type;
  warray * parent;
  int index;
  };

wdeclare_base ( warray );

#define wval_cast_kind_integer( expr ) ( expr )
#define wval_cast_kind_uinteger( expr ) ( expr )
#define wval_cast_kind_floating( expr ) ( expr )
#define wval_cast_kind_pointer( expr ) ( ( wobj * ) ( expr ) )

werror * warray_init ( warray * self, wtype * warray_elem_type );
werror * warray_init_to_size ( warray * self, wtype * warray_elem_type, long num_elements );

warray * warray_new ( wtype * warray_elem_type );
warray * warray_new_to_size ( wtype * warray_elem_type, long num_elements );

void warray_deinit ( warray * self );
void warray_delete ( warray * self );

bool warray_empty ( warray * self );
bool warray_full ( warray * self );

werror * warray_push_back ( warray * self, wval elem );
werror * warray_push_front ( warray * self, wval elem );

werror * warray_pop_back ( warray * self, wval * elem );
werror * warray_pop_front ( warray * self, wval * elem );

long warray_length ( warray * self );
bool warray_good_index ( warray * self, long index );
werror * warray_index ( warray * self, long index, wval ** elem );
werror * warray_remove ( warray * self, long index );
werror * warray_get ( warray * self, long index, wval * elem );
werror * warray_set ( warray * self, long index, wval val );

warray_iter warray_start ( warray * parent );
warray_iter warray_end ( warray * parent );

void warray_prev ( warray_iter * self );
void warray_next ( warray_iter * self );

bool warray_good ( warray_iter * self );
wval warray_deref ( warray_iter * self );
wval * warray_at ( warray_iter * self, long rel_idx );
bool warray_good_at ( warray_iter * self, long rel_idx );
werror * warray_remove_at ( warray_iter * self, long rel_idx );


long warray_index_of ( warray * self, wval to_find );

void warray_debug_print ( warray * self );

#endif /* end of include guard: WARRAY_INTERNAL_H */
