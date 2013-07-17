#include <assert.h>
#include <stdio.h>
#include <werror.h>
#include <wbuiltins.h>

#define warray_elem_t int
#define warray_elem_kind integer
#include <warray.h>

int main ( ) {
  wbuiltins_init ( );
  werror * e = w_ok;
  warray_int a;
  werr ( e, warray_int_init ( &a ) );
  int i = 100;
  int i2;
  werr ( e, warray_int_push_back ( &a, i ) );
  werr ( e, warray_int_pop_front ( &a, &i2 ) );
  assert ( i2 == 100 );
  return e != w_ok;
  }
