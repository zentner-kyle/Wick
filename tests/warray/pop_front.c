#include <assert.h>
#include <stdio.h>
#include <wcall.h>
#include <wbuiltins.h>

#define elem_t int
#include <warray.h>

bool error = false;

wstatus report_error ( wval ignored ) {
  error = true;
  return W_ERROR;
  }

int main ( ) {
  wbuiltins_init ( );
  warray_int a;
  wcall error_wcall = { .func = &report_error, .data = { 0 }, 0, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } };
  warray_int_init ( &a, &error_wcall );
  int i = 100;
  warray_int_push_front ( &a, i, &error_wcall );
  int i2 = 0;
  i2 = warray_int_pop_front ( &a, &error_wcall );
  assert ( i2 == 100 );
  return error;
  }
