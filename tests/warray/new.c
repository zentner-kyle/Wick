#include <assert.h>
#include <stdio.h>
#include <wcall.h>
#include <wbuiltins.h>

#define elem_t int
#include <warray.h>

bool error = false;

wstatus report_error ( ) {
  error = true;
  return W_ERROR;
  }

int main ( ) {
  wbuiltins_init ( );
  wcall error_wcall = null_wcall;
  error_wcall.func = report_error;
  warray_int * a = warray_int_new ( &error_wcall );
  printf ( "%p\n", a->array.data );
  return error;
  }
