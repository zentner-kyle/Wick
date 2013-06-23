#include <wcall.h>
#include <stdlib.h>

const int W_OK = 0;
const int W_ERROR = 1;

wstatus winvoke_0 ( wcall * c ) {
  if ( c && c->func ) {
    if ( c->func ) {
      return c->func ( c->data );
      }
    else {
      return W_OK;
      }
    }
  return W_ERROR;
  }

wcall static_wcall ( wcall_func_t func, wcall_data_t data ) {
  wcall c = { func, data, 0, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } };
  return c;
  }

wcall null_wcall = { 0, { .p = 0 }, 0, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } };
