#include <wmacros.h>
#include <wcall.h>
#include <wtoken.h>
#include <wstr_trie.h>

#define warray_elem_t wcall_ptr
#define warray_elem_kind pointer
#include <warray.h>

#define warray_elem_t wtoken_ptr
#define warray_elem_kind pointer
#include <warray.h>

def_struct ( wparser ) {
  wtype * type;
  wstr all_text;
  wstr text;
  wcall * handle_error;
  wstr_trie * token_table;
  warray_wtoken_ptr * tokens;
  warray_wcall_ptr * lexers;
  warray_wcall_ptr * parsers;
  };

wdeclare_composite ( wparser );
