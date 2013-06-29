#include <wmacros.h>
#include <wcall.h>
#include <wtoken.h>

#define warray_elem_t wcall_ptr
#define warray_elem_kind pointer
#include <warray.h>

#define warray_elem_t wtoken_ptr
#define warray_elem_kind pointer
#include <warray.h>

def_struct ( parser ) {
  wtype * type;
  wstr text;
  wstr remaining;
  wcall * handle_error;
  warray_wtoken_ptr * tokens;
  warray_wcall_ptr * lexers;
  warray_wcall_ptr * parsers;
  };



