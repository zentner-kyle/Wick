#include <assert.h>
#include <stdio.h>
#include "warray.h"
#include "wcall.h"
#include "wbuiltins.h"

bool error = false;

void report_error( void * ignored ) {
  error = true;
}

int main() {
  wbuiltins_init();
  wcall error_wcall = { .func = report_error, .data = NULL };
  warray * a = warray_new( wtype_upcast(&wtype_int), error_wcall );
  int i = 1;
  warray_push_front( a, &i, error_wcall );
  ++i;
  warray_push_front( a, &i, error_wcall );
  ++i;
  warray_push_front( a, &i, error_wcall );
  i = 2;
  assert( *(int *)warray_find( a, &i ) == 2 );
  printf( "%p\n", a->data );
  return error;
}
