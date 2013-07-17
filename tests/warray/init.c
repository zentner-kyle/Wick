#include <assert.h>
#include <werror.h>
#include <wbuiltins.h>

#define warray_elem_t int
#define warray_elem_kind integer
#include <warray.h>

int main ( ) {
  wbuiltins_init ( );
  warray_int a;
  return w_ok != warray_int_init ( &a );
  }
