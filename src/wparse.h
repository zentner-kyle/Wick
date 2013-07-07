#include <wmacros.h>
#include <wcall.h>
#include <wtoken.h>
#include <wstr_trie.h>
#include <werror.h>
#include <wobj.h>

#define warray_elem_t wcall_ptr
#define warray_elem_kind pointer
#include <warray.h>

#define warray_elem_t wobj_ptr
#define warray_elem_kind pointer
#include <warray.h>

def_struct ( wparser ) {
  wtype * type;
  wstr all_text;
  wstr text;
  wcall * handle_error;
  wstr_trie * token_table;
  warray_wobj_ptr * tokens;
  bool stop;
  };

wdeclare_composite ( wparser );

void wparser_error ( wparser * self, werror * error );
bool wparser_push_token ( wparser * self, wtoken * token );

wparser * wparser_new ( wstr * text );

werror * wparser_lex ( wparser * self );

void wparser_print_tokens ( wparser * self );
