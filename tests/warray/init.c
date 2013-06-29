#include <assert.h>
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
  warray_int a;
  wcall error_wcall = null_wcall;
  error_wcall.func = report_error;
  warray_int_init ( &a, &error_wcall );
  return error;
  }
