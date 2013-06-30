#include <assert.h>
#include <stdbool.h>
#include <wbuiltins.h>
#include <wstr.h>
#include <wtoken.h>
#include <walloc.h>
#include <wtype.h>
#include <wval.h>
#include <wparse.h>

bool wbuiltins_initialized = false;

wdefine_base ( bool );
wdefine_base ( char );
wdefine_base ( double );
wdefine_base ( float );
wdefine_base ( int );
wdefine_base ( long );
wdefine_base ( long_double );
wdefine_base ( long_long );
wdefine_base ( short );
wdefine_base ( size_t );
wdefine_base ( unsigned );
wdefine_base ( unsigned_long );
wdefine_base ( unsigned_short );
wdefine_base ( wval );


void wbuiltins_init ( ) {
  if ( wbuiltins_initialized ) {
    return;
    }
  #define walloc_error() \
    return;
  winit_base ( bool );
  winit_base ( char );
  winit_base ( double );
  winit_base ( float );
  winit_base ( int );
  winit_base ( long );
  winit_base ( long_double );
  winit_base ( long_long );
  winit_base ( short );
  winit_base ( size_t );
  winit_base ( unsigned );
  winit_base ( unsigned_long );
  winit_base ( unsigned_short );
  winit_base ( wval );

  winit_composite ( wstr );
  winit_composite ( wtoken );
  winit_composite ( wtype );
  winit_composite ( wcall );

  #define wcall_name add_token
  #define wcall_wtypes wtype_int, wtype_int, wtype_int, wtype_wstr_ptr, wtype_wtoken_ptr_ptr
  #include <wcall_init.c>
  }

#undef init_builtin
