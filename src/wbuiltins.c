#include <assert.h>
#include <stdbool.h>
#include <wbuiltins.h>
#include <wstr.h>
#include <wtoken.h>
#include <walloc.h>
#include <wtype.h>
#include <wval.h>
#include <wparse.h>
#include <wstr_trie.h>
#include <werror.h>
#include <wast.h>

bool wbuiltins_initialized = false;

wdefine_base ( wval );
wdefine_composite ( wobj );

wdefine_base ( size_t );
wdefine_base ( int );

#ifndef WICK_MICRO

wdefine_base ( bool );
wdefine_base ( char );
wdefine_base ( double );
wdefine_base ( float );
wdefine_base ( long );
wdefine_base ( long_double );
wdefine_base ( long_long );
wdefine_base ( short );
wdefine_base ( unsigned );
wdefine_base ( unsigned_long );
wdefine_base ( unsigned_short );

/*#ifndef WICK_MICRO*/
wdefine_composite ( wast );
wdefine_composite ( wast_prefix );
wdefine_composite ( wast_infix );
wdefine_composite ( wast_list );

wdefine_composite ( wtoken );
wdefine_composite ( wtoken_indent );
wdefine_composite ( wtoken_infix );
wdefine_composite ( wtoken_prefix );
wdefine_composite ( wtoken_left );
wdefine_composite ( wtoken_right );
#endif /* WICK_MICRO */

void wbuiltins_init ( ) {
  if ( wbuiltins_initialized ) {
    return;
    }
  #define walloc_error() \
    return;
  winit_base ( size_t );
  winit_base ( int );
  winit_base ( wval );

  winit_composite ( wstr );
  winit_composite ( wtype );
  winit_composite ( wcall );
  winit_composite ( werror );
  winit_composite ( wobj );

#ifndef WICK_MICRO

  winit_base ( bool );
  winit_base ( char );
  winit_base ( double );
  winit_base ( float );
  winit_base ( long );
  winit_base ( long_double );
  winit_base ( long_long );
  winit_base ( short );
  winit_base ( unsigned );
  winit_base ( unsigned_long );
  winit_base ( unsigned_short );

  winit_composite ( wstr_trie );
  winit_composite ( wtoken );

  winit_composite ( wast );
  winit_composite ( wast_prefix );
  winit_composite ( wast_infix );
  winit_composite ( wast_list );

  winit_composite ( wtoken );
  winit_composite ( wtoken_indent );
  winit_composite ( wtoken_infix );
  winit_composite ( wtoken_prefix );
  winit_composite ( wtoken_left );
  winit_composite ( wtoken_right );
#endif /* WICK_MICRO */

  }

#undef init_builtin
