#include <wint_trie.h>

int main (void) {
  wint_trie * t = wint_trie_new ( );
  if ( t != NULL ) {
    return 0;
    }
  else {
    return 1;
    }
  }
