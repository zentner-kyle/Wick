#include <wmacros.h>
#include <wcall.h>
#include <wtoken.h>
#include <wstr_trie.h>
#include <werror.h>
#include <wobj.h>
#include <wast.h>

#define warray_elem_t wcall_ptr
#define warray_elem_kind pointer
#include <warray.h>

enum wparser_state {
  wparser_expr_complete,
  wparser_op_pending
  };

def_struct ( wparser ) {
  wtype * type;
  wstr all_text;
  wstr text;
  wstr_trie * token_table;
  wast * accum;
  wast_list * root;
  wtoken * last_token;
  bool stop;
  enum wparser_state state;
  };

wdeclare_composite ( wparser );

void wparser_error ( wparser * self, werror * error );
bool wparser_push_token ( wparser * self, wtoken * token );

wparser * wparser_new ( wstr * text );

werror * wparser_lex ( wparser * self );

//void wparser_print_tokens ( wparser * self );
