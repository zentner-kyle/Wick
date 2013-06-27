#ifndef WBUILTINS_H
#define WBUILTINS_H
#include <stdbool.h>
#include <wtype.h>
#include <wmacros.h>

/* Handle bool being a macro which will be incorrectly expanded. */

#ifdef bool
  typedef bool w_bool;
  #undef bool
  typedef w_bool bool;
  #endif

typedef long long long_long;
typedef long double long_double;
typedef unsigned long unsigned_long;
typedef unsigned short unsigned_short;

void wbuiltins_init ( );

wdeclare_base ( bool );
wdeclare_base ( char );
wdeclare_base ( double );
wdeclare_base ( float );
wdeclare_base ( int );
wdeclare_base ( long );
wdeclare_base ( long_double );
wdeclare_base ( long_long );
wdeclare_base ( short );
wdeclare_base ( unsigned );
wdeclare_base ( unsigned_long );
wdeclare_base ( unsigned_short );

#endif /* end of include guard: WBUILTINS_H */
