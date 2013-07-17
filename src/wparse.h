#include <wmacros.h>
#include <wcall.h>
#include <wtoken.h>
#include <wstr_trie.h>
#include <werror.h>
#include <wobj.h>
#include <wast.h>

#define wtable_key_t wstr_ptr
#define wtable_key_kind pointer

#define wtable_val_t wtoken_ptr
#define wtable_val_kind pointer
#include <wtable.h>

enum wparser_state {
  wparser_prefix_context,
  wparser_infix_context
  };

def_struct ( wparser ) {
  wtype * type;
  wstr all_text;
  wstr text;
  wstr_trie * prefix_table;
  wstr_trie * infix_table;
  wtable_wstr_ptr_to_wtoken_ptr * literal_table;
  wtable_wstr_ptr_to_wtoken_ptr * identifier_table;
  wtable_wstr_ptr_to_wtoken_ptr * indent_table;
  wast * accum;
  wast_list * root;
  wtoken * last_token;
  bool stop;
  enum wparser_state state;
  };

wdeclare_composite ( wparser );

wparser * wparser_new ( wstr * text );

werror * wparser_parse ( wparser * self );
