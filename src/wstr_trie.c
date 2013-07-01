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
    node = wstr_trie_enter_next ( node, *c );
    if ( ! node ) {
      return NULL;
      }
    ++c;
    }
    return node;
  }

wstr_trie * wstr_trie_enter_next ( wstr_trie * node, char cc ) {
  for (int i = 0; i < sizeof ( char ) / WSTR_TRIE_SHIFT; i++) {
    if ( ! node->next[cc & WSTR_TRIE_MASK] ) {
      node->next[cc & WSTR_TRIE_MASK] = (wstr_trie *) calloc ( sizeof ( wstr_trie ), 1 );
      if ( ! node->next[cc & WSTR_TRIE_MASK] ) {
        return NULL;
        }
      node->next[cc & WSTR_TRIE_MASK]->type = wtype_wstr_trie;
      node->next[cc & WSTR_TRIE_MASK]->prev = node;
      }
      node = node->next[cc & WSTR_TRIE_MASK];
      cc >>= WSTR_TRIE_SHIFT;
    }
    return node;
  }

wstr_trie * wstr_trie_get_next ( wstr_trie * node, char cc ) {
  for (int i = 0; i < sizeof ( char ) / WSTR_TRIE_SHIFT; i++) {
    if ( ! node->next[cc & WSTR_TRIE_MASK] ) {
      return node;
      }
      node = node->next[cc & WSTR_TRIE_MASK];
      cc >>= WSTR_TRIE_SHIFT;
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
    wstr_trie * prev_node = node
    node = wstr_trie_get_next ( node, *c );
    if ( node == prev_node ) {
      return node;
      }
    ++c;
    }
    return node;
  }

