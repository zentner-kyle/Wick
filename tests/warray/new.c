#include <assert.h>
#include <stdio.h>
#include <werror.h>
#include <wbuiltins.h>

#define warray_elem_t int
#define warray_elem_kind integer
#include <warray.h>

int main ( ) {
  wbuiltins_init ( );
  warray_int * a = warray_int_new ( );
  printf ( "a = %p\n", a );
  fflush ( stdout );
  printf ( "a->array.data = %p\n", a->array.data );
  return 0;
  }
