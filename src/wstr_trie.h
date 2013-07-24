#ifndef WICK_MICRO
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
  wstr_trie * prev;
  wobj * val;
  };

wdeclare_composite ( wstr_trie );

wstr_trie * wstr_trie_enter ( wstr_trie * node, wstr * str );

wstr_trie * wstr_trie_enter_next ( wstr_trie * node, char c );

wstr_trie * wstr_trie_get_longest ( wstr_trie * node, wstr * str );

wstr_trie * wstr_trie_get_next ( wstr_trie * node, char c );

wstr_trie * wstr_trie_new ( void );

void wstr_trie_print ( wstr_trie * node );

void wstr_trie_free ( wstr_trie * );

#endif /* end of include guard: WSTR_TRIE_H */
#endif /* WICK_MICRO */
