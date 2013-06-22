#include <assert.h>
#include <stdio.h>
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
  wcall error_wcall = { .func = report_error, .data = NULL   };
  warray_int_init_to_size ( &a, 60, error_wcall );
  /*assert ( a.array.space == 64 );*/
  return error;
  }
