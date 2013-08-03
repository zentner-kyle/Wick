#include <stdlib.h>
#include <wmacros.h>
#include <werror.h>
#include <walloc.h>
#include <stdbool.h>

/*typedef uint64_t wint_trie_int;*/
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

def_struct (wint_trie) {
  wtype * type;
  wint_trie_node * root;
  };

wdeclare_composite ( wint_trie );

