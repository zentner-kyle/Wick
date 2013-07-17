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
  warray_int_iter iter = warray_int_start ( &a );
  int i = 100;
  werr ( e, warray_int_push_back ( &a, i ) );
  int i2 = 0;
  werr ( e, warray_int_pop_back ( &a, &i2 ) );
  printf ( "i2 = %d\n", i2 );
  assert ( i2 == 100 );
  assert ( ! warray_int_good ( &iter ) );
  assert ( warray_int_length ( &a ) == 0 );
  return e != w_ok;
  }
