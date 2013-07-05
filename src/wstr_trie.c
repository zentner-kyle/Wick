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
  printf ( "enter next %c\n", cc );
  for (int i = 0; i < (sizeof ( char ) * 8) / WSTR_TRIE_SHIFT; i++) {
    printf ( "index = %d\n", cc & WSTR_TRIE_MASK );
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
  wstr_trie * prev_node = node;
  for (int i = 0; i < (sizeof ( char ) * 8) / WSTR_TRIE_SHIFT; i++) {
    if ( ! node->next[cc & WSTR_TRIE_MASK] ) {
      return prev_node;
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
    wstr_trie * prev_node = node;
    node = wstr_trie_get_next ( node, *c );
    if ( node == prev_node ) {
      return node;
      }
    ++c;
    }
    return node;
  }

wstr_trie * wstr_trie_new ( void ) {
  wstr_trie * self = ( wstr_trie * ) calloc ( sizeof ( wstr_trie ), 1 );
  if ( self ) {
    self->type = wtype_wstr_trie;
    }
  return self;
  }

void wstr_trie_free ( wstr_trie * self ) {
  if ( self ) {
    for (int i = 0; i < WSTR_TRIE_BRANCH; i++) {
      wstr_trie_free ( self->next[i] );
      }
    }
  free ( self );
  }

#include <wtoken.h>

void wstr_trie_print_recursive ( wstr_trie * node, int indent ) {
  for (int i = 0; i < indent; i++) {
    printf ( "  " );
    }
  printf (".val = %p\n", node->val );
  for (int i = 0; i < indent; i++) {
    printf ( "  " );
    }
  for (int i = 0; i < WSTR_TRIE_BRANCH; i++) {
    if ( ! node->next[i] ) {
      printf ( "." );
      }
    else {
      printf ("\n");
      wstr_trie_print_recursive ( node->next[i], indent + 1 );
      }
    }
  }

void wstr_trie_print ( wstr_trie * node ) {
  wstr_trie_print_recursive ( node, 0 );
  printf ( "\n" );
  }
