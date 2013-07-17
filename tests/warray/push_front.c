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
  werr ( e, warray_int_push_front ( &a, i ) );
  werr ( e, warray_int_get ( &a, 0, &i ) );
  assert ( i == 100 );
  return e != w_ok;
  }
