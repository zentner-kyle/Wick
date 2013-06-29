#ifndef WSTR_H
#define WSTR_H
#include <stdio.h>
#include <stdint.h>
#include <wmacros.h>
#include <wtype_h.h>
#include <whash.h>
#include <wtable_wval.h>

enum wstr_alloc_type {
  wstr_dynamic = 1,
  wstr_static = 2,
  wstr_gc = 3
  };

def_struct ( wstr ) {
  wtype * type;
  const char * start;
  const char * past_end;
  enum wstr_alloc_type alloc_type;
  };

wdeclare_composite ( wstr );

extern wtable_elem_interface wstr_wtable_i;

#define wstr_lit( literal ) \
  ( wstr ) { wtype_wstr, literal, literal + sizeof ( literal ) - 1, wstr_static }

wstr * wstr_new ( const char * start, const char * past_end );

wstr wstr_from_static ( const char * literal );

wstr wstr_from_dynamic ( const char * dynamic );

wstr wstr_from_filename ( wstr filename );

wstr wstr_from_file ( FILE * file );

int wstr_compare ( wstr a, wstr b );

void wstr_init_dynamic ( wstr * to_init );

void wstr_print ( wstr str );

void wstr_println ( wstr to_print );

size_t wstr_size ( const wstr str );

whash wstr_hash ( wstr str );

wstr * wstr_new ( const char * start, const char * past_end );
wstr * wstr_new_alloc ( const char * start, const char * past_end, enum wstr_alloc_type alloc_type );
void wstr_init ( wstr * self, const char * start, const char * past_end );
void wstr_init_alloc ( wstr * self, const char * start, const char * past_end, enum wstr_alloc_type alloc_type );

wstr * wstr_from_llong ( long long input );

int wcompare_wstr_ptr ( wval v_str_a, wval v_str_b );
whash whash_wstr_ptr ( wval str );

#endif /* end of include guard: WSTR_H */
