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
  werr ( e, warray_int_push_front ( a, 1 ) );
  werr ( e, warray_int_push_front ( a, 2 ) );
  werr ( e, warray_int_push_front ( a, 3 ) );
  assert ( warray_int_index_of ( a, 2 ) == 1 );
  return e != w_ok;
  }
