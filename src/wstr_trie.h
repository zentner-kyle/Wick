#ifndef WSTR_TRIE_H

#define WSTR_TRIE_H

#include <wstr.h>
#include <wmacros.h>

#define WSTR_TRIE_BRANCH 0x10
#define WSTR_TRIE_MASK 0xf
#define WSTR_TRIE_SHIFT 4

def_struct ( wstr_trie ) {
  wtype * type;
  wstr_trie * next[WSTR_TRIE_BRANCH];
  wobj * val;
  };

wdeclare_composite ( wstr_trie );

wstr_trie * wstr_trie_enter ( wstr_trie * node, wstr * str );

wstr_trie * wstr_trie_get_longest ( wstr_trie * node, wstr * str );

#endif /* end of include guard: WSTR_TRIE_H */
