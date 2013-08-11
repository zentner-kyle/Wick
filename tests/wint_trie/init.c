#include <wint_trie.h>

int main (void) {
  wint_trie t;
  if ( wint_trie_init ( &t ) == w_ok ) {
    return 0;
    }
  else {
    return 1;
    }
  }
