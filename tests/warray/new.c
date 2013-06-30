#include <assert.h>
#include <stdio.h>
#include <wcall.h>
#include <wbuiltins.h>

#define warray_elem_t int
#define warray_elem_kind integer
#include <warray.h>

bool error = false;

werror * report_error ( wval * ignored ) {
  error = true;
  return &werror_generic;
  }

int main ( ) {
  wbuiltins_init ( );
  wcall error_wcall = null_wcall;
  error_wcall.func = report_error;
  warray_int * a = warray_int_new ( &error_wcall );
  printf ( "%p\n", a->array.data );
  return error;
  }
