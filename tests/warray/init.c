#include <assert.h>
#include <wcall.h>
#include <wbuiltins.h>

#define elem_t int
#include <warray.h>

bool error = false;

void report_error ( void * ignored ) {
  error = true;
  }

int main ( ) {
  wbuiltins_init ( );
  warray_int a;
  wcall error_wcall = { .func = report_error, .data = NULL };
  warray_int_init ( &a, error_wcall );
  return error;
  }
