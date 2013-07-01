#include <wmacros.h>
#include <wcall.h>
#include <wtoken.h>
#include <wstr_trie.h>
#include <werror.h>

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
  wstr_trie * short_table;
  wstr_trie * long_table;
  warray_wtoken_ptr * tokens;
  warray_wcall_ptr * lexers;
  warray_wcall_ptr * parsers;
  bool stop;
  };

wdeclare_composite ( wparser );

void wparser_error ( wparser * self, werror * error );
bool wparser_push_token ( wparser * self, wtoken * token );
