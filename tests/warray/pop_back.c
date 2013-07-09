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
  warray_int a;
  wcall error_wcall = null_wcall;
  error_wcall.func = report_error;
  warray_int_init ( &a, &error_wcall );
  warray_int_iter iter = warray_int_start ( &a );
  int i = 100;
  warray_int_push_back ( &a, i, &error_wcall );
  int i2 = 0;
  i2 = warray_int_pop_back ( &a, &error_wcall );
  printf ( "i2 = %d\n", i2 );
  assert ( i2 == 100 );
  assert ( ! warray_int_good ( &iter ) );
  assert ( warray_int_length ( &a ) == 0 );
  return error;
  }
