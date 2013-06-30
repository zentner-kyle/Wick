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
  int i = 100;
  warray_int_push_back ( &a, i, &error_wcall );
  assert ( warray_int_pop_front ( &a, &error_wcall ) == 100 );
  return error;
  }
