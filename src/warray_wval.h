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
  wtype * elem_type;
  size_t space;
  size_t past_end;
  size_t start;
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

#define warray_static_1( elem_type, elem_kind, elem_0 ) \
  ( warray ) { \
    ( wtype * ) &wtype_warray, \
    ( wtype * ) &w_type_to_wtype ( elem_type ), \
    1, \
    1, \
    0, \
    ( wval [] ) { { wval_cast_kind_##elem_kind ( elem_0 ) } } \
    };

#define warray_static_4( elem_type, elem_kind, elem_0, elem_1, elem_2, elem_3 ) \
  ( warray ) { \
    ( wtype * ) &wtype_warray, \
    ( wtype * ) &w_type_to_wtype ( elem_type ), \
    4, \
    4, \
    0, \
    ( wval [] ) { \
        { .elem_kind = wval_cast_kind_##elem_kind ( elem_0 ) }, \
        { .elem_kind = wval_cast_kind_##elem_kind ( elem_1 ) }, \
        { .elem_kind = wval_cast_kind_##elem_kind ( elem_2 ) }, \
        { .elem_kind = wval_cast_kind_##elem_kind ( elem_3 ) }, \
    } };

bool warray_init ( warray * self, wtype * elem_type, wcall * error );
bool warray_init_to_size ( warray * self, wtype * elem_type, size_t num_elements, wcall * error );

warray * warray_new ( wtype * elem_type, wcall * error );
warray * warray_new_to_size ( wtype * elem_type, size_t num_elements, wcall * error );

void warray_deinit ( warray * self );
void warray_delete ( warray * self );

bool warray_empty ( warray * self );
bool warray_full ( warray * self );

void warray_push_back ( warray * self, wval elem, wcall * error );
void warray_push_front ( warray * self, wval elem, wcall * error );
wval warray_pop_back ( warray * self, wcall * error );
wval warray_pop_front ( warray * self, wcall * error );

size_t warray_length ( warray * self );
bool warray_good_index ( warray * self, size_t index );
wval * warray_index ( warray * self, size_t index );
wval warray_get ( warray * self, size_t index );
wval warray_set ( warray * self, size_t index, wval val );

warray_iter warray_start ( warray * parent );
warray_iter warray_end ( warray * parent );

bool warray_prev ( warray_iter * self );
bool warray_next ( warray_iter * self );

bool warray_good ( warray_iter * self );
wval warray_deref ( warray_iter * self );


long warray_index_of ( warray * self, wval to_find );

void warray_debug_print ( warray * self );

#endif /* end of include guard: WARRAY_INTERNAL_H */
