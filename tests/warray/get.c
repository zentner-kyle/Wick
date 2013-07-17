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
  werr ( e, warray_int_push_back ( &a, i ) );
  int i2;
  werr ( e, warray_int_get ( &a, 0, &i2 ) );
  printf ( "i2 = %d\n", i2 );
  assert ( i2 == 100 );
  return e != w_ok;
  }
