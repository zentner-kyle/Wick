#include <wint_trie.h>
#include <wval.h>
#include <stdio.h>

#define INSERT \
  printf ( "Inserting %d\n", i ); \
  fflush ( stdout ); \
  if ( wint_trie_insert ( t, i, wval_int ( i ) ) != w_ok ) { \
    return 1; \
    }

#define GET \
  printf ( "Getting %d\n", i ); \
  fflush ( stdout ); \
  if ( wint_trie_get ( t, i, &val ) != w_ok ) { \
    return 1; \
    } \
  if ( val->integer != i ) { \
    printf ( "Got " wint_format " instead of %d.\n", val->integer, i ); \
    wint_trie_debug_print ( t->root, 1 ); \
    fflush ( stdout ); \
    return 1; \
    }

int main (void) {
  wint_trie * t = wint_trie_new ( );
  wval * val;
  if ( t == NULL ) {
    return 255;
    }
  /*for ( int i = 0; i <= 10; i++ ) {*/
    /*INSERT;*/
    /*GET;*/
    /*}*/
  /*int i = 4;*/
  /*INSERT;*/
  /*GET;*/

  /*for ( int i = 0; i <= 3; i++ ) {*/
    /*INSERT;*/
    /*GET;*/
    /*}*/

  /*for ( int i = 10; i > 7; i-- ) {*/
    /*INSERT;*/
    /*GET;*/
    /*}*/
  for ( int i = 10; i >= 8; i-- ) {
    INSERT;
    GET;
    }
  for ( int i = 8; i <= 10; i += 1 ) {
    GET;
    }

  /*for ( int i = 20; i > 17; i-- ) {*/
    /*INSERT;*/
    /*GET;*/
    /*}*/
  /*for ( int i = 21; i <= 30; i += 2 ) {*/
    /*INSERT;*/
    /*GET;*/
    /*}*/
  /*for ( int i = 21; i > 20; i -= 2 ) {*/
    /*INSERT;*/
    /*GET;*/
    /*}*/
  /*wint_trie_debug_print ( t->root, 1 );*/
  /*for ( int i = 0; i <= 10; i++ ) {*/
    /*GET;*/
    /*}*/
  /*for ( int i = 20; i > 10; i-- ) {*/
    /*GET;*/
    /*}*/
  /*for ( int i = 29; i > 20; i -= 2 ) {*/
    /*GET;*/
    /*}*/
  return 0;
  }
