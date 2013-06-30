#include <wmacros.h>
#include <wcall.h>
#include <wtoken.h>

#define warray_elem_t wcall_ptr
#define warray_elem_kind pointer
#include <warray.h>

#define warray_elem_t wtoken_ptr
#define warray_elem_kind pointer
#include <warray.h>

#define wtable_key_t wstr_ptr
#define wtable_key_kind pointer

#define wtable_val_t wtoken_ptr
#define wtable_val_kind pointer
#include <wtable.h>

def_struct ( wparser ) {
  wtype * type;
  wstr all_text;
  wstr text;
  wcall * handle_error;
  wtable_wstr_ptr_to_wtoken_ptr * token_table;
  warray_wtoken_ptr * tokens;
  warray_wcall_ptr * lexers;
  warray_wcall_ptr * parsers;
  };

wdeclare_composite ( wparser );

wstatus add_token (
    int family,
    int lbp,
    int rbp,
    wstr ** text,
    wtoken ** to_store
  );

#define wcall_name add_token
#define wcall_argc 5
#include <wcall_declare.h>

