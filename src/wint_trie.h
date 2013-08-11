#include <stdlib.h>
#include <wmacros.h>
#include <werror.h>
#include <walloc.h>
#include <stdbool.h>
#include <wval.h>

typedef size_t wint_trie_int;

declare_struct ( wint_trie_node );

def_union ( wint_trie_child_t ) {
  wint_trie_node * pointer;
  wint_trie_int    integer;
  };

struct wint_trie_node {
  wint_trie_child_t children[2];
  wint_trie_int mask; /* If bottom bit is set, children both contain leaves. */
  wint_trie_int prefix;
  };

def_struct( wint_trie_leaf ) {
  wint_trie_child_t children[2];
  wint_trie_int mask; /* If bottom bit is set, children both contain leaves. */
  wint_trie_int prefix;
  wval values[2];
  };

def_struct (wint_trie) {
  wtype * type;
  wint_trie_node * root;
  };

wdeclare_composite ( wint_trie );

werror * wint_trie_init ( wint_trie * t );
wint_trie * wint_trie_new ( void );
werror * wint_trie_insert ( wint_trie * t, wint_trie_int i, wval val );
werror * wint_trie_get ( wint_trie * t, wint_trie_int i, wval ** val );
void wint_trie_debug_print ( wint_trie_node * n, int indent );
