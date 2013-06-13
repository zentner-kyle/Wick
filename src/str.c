#include "str.h"
#include <stdio.h>
#include <string.h>

size_t str_print( str s ) {
  return fwrite( s.start, sizeof(char), s.len, stdout );
}

size_t str_println( str s ) {
  size_t e = fwrite( s.start, sizeof(char), s.len, stdout );
  if ( e == s.len ) {
    putchar( '\n' );
    return e + 1;
  }
  return e;
}

bool str_equal( str a, str b ) {
  return a.len == b.len && ! memcmp( a.start, b.start, a.len );
}

str str_from_c(char * c) {
  return (str){.start = c, .len = strlen(c)};
}
