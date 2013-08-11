#include <wint_trie.h>
#include <wval.h>
#include <stdio.h>

int main (void) {
  wint_trie * t = wint_trie_new ( );
  if ( t == NULL ) {
    return 255;
    }
  for ( int i = 0; i <= 100; i++ ) {
    printf ( "Inserting %d\n", i );
    fflush ( stdout );
    if ( wint_trie_insert ( t, i, wval_int ( i ) ) != w_ok ) {
      return 1;
      }
    }
  for ( int i = 200; i > 100; i--) {
    printf ( "Inserting %d\n", i );
    fflush ( stdout );
    if ( wint_trie_insert ( t, i, wval_int ( i ) ) != w_ok ) {
      return 1;
      }
    }
  for ( int i = 210; i <= 300; i += 10 ) {
    printf ( "Inserting %d\n", i );
    fflush ( stdout );
    if ( wint_trie_insert ( t, i, wval_int ( i ) ) != w_ok ) {
      return 1;
      }
    }
  for ( int i = 295; i > 200; i -= 10) {
    printf ( "Inserting %d\n", i );
    fflush ( stdout );
    if ( wint_trie_insert ( t, i, wval_int ( i ) ) != w_ok ) {
      return 1;
      }
    }
  wint_trie_debug_print ( t->root, 1 );
  return 0;
  }
