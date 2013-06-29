#include <assert.h>
#include <stdio.h>
#include <wcall.h>
#include <wbuiltins.h>

#define elem_t int
#include <warray.h>

bool error = false;

wstatus report_error ( wval * ignored ) {
  error = true;
  return W_ERROR;
  }

int main ( ) {
  wbuiltins_init ( );
  wcall error_wcall = null_wcall;
  error_wcall.func = report_error;
  warray_int * a = warray_int_new ( &error_wcall );
  warray_int_push_front ( a, 1, &error_wcall );
  warray_int_push_front ( a, 2, &error_wcall );
  warray_int_push_front ( a, 3, &error_wcall );
  assert ( warray_int_index_of ( a, 2 ) == 1 );
  return error;
  }
