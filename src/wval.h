#ifndef WVAL_H

#define WVAL_H
#include <stdint.h>

#include <wobj.h>
#include <wmacros.h>

#ifdef WICK_MICRO
typedef int wint;
#define wint_format "%d"
typedef unsigned wuint;
#define wuint_format "%u"
typedef float wfloat;
#define wfloat_format "%f"
#else
typedef int64_t wint;
#define wint_format "%ld"
typedef uint64_t wuint;
#define wuint_format "%zd"
typedef double wfloat;
#define wfloat_format "%f"
#endif


def_union ( wval ) {
  wint integer;
  wuint uinteger;
  wfloat floating;
  wobj * pointer;
  };

wdeclare_base ( wval );

#define wval_int( val ) ( ( wval ) { .integer = ( wint ) val } )
#define wval_uint( val ) ( ( wval ) { .uinteger = ( wuint ) val } )
#define wval_float( val ) ( ( wval ) { .floating = ( wfloat ) val } )
#define wval_pointer( val ) ( ( wval ) { .pointer = ( wobj * ) val } )


#endif /* end of include guard: WVAL_H */
