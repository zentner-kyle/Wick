#ifndef STR_H
#define STR_H
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct str {
  char * start;
  size_t len; /* Doesn't count null character, may not be null terminated. */
  };

typedef struct str str;

size_t str_print ( str s );

size_t str_println ( str s );

bool str_equal ( str a, str b );

str str_from_c ( char * c );


#endif /* end of include guard: STR_H */
