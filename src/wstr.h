#ifndef WSTR_H
#define WSTR_H
#include <stdio.h>
#include <stdint.h>
#include <wmacros.h>
#include <wtype_h.h>
#include <whash.h>
#include <wtable.h>

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

extern wtype wstr_type;
extern wtable_elem_interface wstr_wtable_i;

#define WSTR_LIT( literal ) \
  ( wstr ) { &wstr_type, literal, literal + sizeof ( literal ) - 1, wstr_static   }

wstr * wstr_new ( const char * start, const char * past_end );

wstr wstr_from_literal ( const char * literal );

wstr wstr_from_dynamic ( const char * dynamic );

wstr wstr_from_filename ( wstr filename );

wstr wstr_from_file ( FILE * file );

int wstr_compare ( wstr a, wstr b );

void wstr_init_dynamic ( wstr * to_init );

void wstr_print ( wstr str );

void wstr_println ( wstr to_print );

size_t wstr_size ( const wstr str );

whash_t wstr_hash ( wstr str );

wstr * wstr_new ( const char * start, const char * past_end );
wstr * wstr_new_alloc ( const char * start, const char * past_end, enum wstr_alloc_type alloc_type );
void wstr_init ( wstr * self, const char * start, const char * past_end );
void wstr_init_alloc ( wstr * self, const char * start, const char * past_end, enum wstr_alloc_type alloc_type );

wstr * wstr_from_llong ( long long input );

#endif /* end of include guard: WSTR_H */
