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
  warray_int * a = warray_int_new ( );
  warray_int_iter i = warray_int_start ( a );
  werr ( e, warray_int_push_front ( a, 1 ) );
  werr ( e, warray_int_remove_at ( &i, 0 ) );
  assert ( ! warray_int_good ( &i ) );
  assert ( warray_int_length ( a ) == 0 );
  return e != w_ok;
  }
