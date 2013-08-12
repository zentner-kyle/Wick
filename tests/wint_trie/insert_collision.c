#include <wint_trie.h>
#include <wval.h>
#include <stdio.h>

#define INSERT \
  printf ( "Inserting %d\n", i ); \
  fflush ( stdout ); \
  if ( wint_trie_insert ( t, i, wval_int ( i ) ) != w_ok ) { \
    return 1; \
    }

int insert_a_bunch ( wint_trie * t ) {
  for ( int i = 0; i <= 10; i++ ) {
    INSERT;
    }
  for ( int i = 20; i > 10; i--) {
    INSERT;
    }
  for ( int i = 21; i <= 30; i += 2 ) {
    INSERT;
    }
  for ( int i = 30; i >= 22; i -= 2) {
    INSERT;
    }
  return 0;
  }

int main (void) {
  wint_trie * t = wint_trie_new ( );
  if ( t == NULL ) {
    return 255;
    }
  insert_a_bunch ( t );
  insert_a_bunch ( t );
  return 0;
  }
