#include <wstr_trie.h>
#include <wmacros.h>
#include <stdlib.h>
#include <assert.h>

wdefine_composite ( wstr_trie );

wstr_trie * wstr_trie_enter ( wstr_trie * node, wstr * str ) {
  assert ( WSTR_TRIE_BRANCH - 1 == WSTR_TRIE_MASK );
  if ( ! node || ! str ) {
    return NULL;
    }
  const char * c = str->start;
  while ( c != str->past_end ) {
    char cc = *c;
    for (int i = 0; i < sizeof ( char ) / WSTR_TRIE_SHIFT; i++) {
      if ( ! node->next[cc & WSTR_TRIE_MASK] ) {
        node->next[cc & WSTR_TRIE_MASK] = (wstr_trie *) calloc ( sizeof ( wstr_trie ), 1 );
        if ( ! node->next[cc & WSTR_TRIE_MASK] ) {
          return NULL;
          }
        node->next[cc & WSTR_TRIE_MASK]->type = wtype_wstr_trie;
        }
        node = node->next[cc & WSTR_TRIE_MASK];
        cc >>= WSTR_TRIE_SHIFT;
      }
      ++c;
    }
    return node;
  }

wstr_trie * wstr_trie_get_longest ( wstr_trie * node, wstr * str ) {
  assert ( WSTR_TRIE_BRANCH - 1 == WSTR_TRIE_MASK );
  if ( ! node || ! str ) {
    return NULL;
    }
  const char * c = str->start;
  while ( c != str->past_end ) {
    char cc = *c;
    for (int i = 0; i < sizeof ( char ) / WSTR_TRIE_SHIFT; i++) {
      if ( ! node->next[cc & WSTR_TRIE_MASK] ) {
        return node;
        }
        node = node->next[cc & WSTR_TRIE_MASK];
        cc >>= WSTR_TRIE_SHIFT;
      }
      ++c;
    }
    return node;
  }

