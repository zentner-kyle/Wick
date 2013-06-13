#include <stdio.h>

const int last_val = ~0;
#define array_last_val last_val
#define array_t int
#include <def_array.h>
#include <def_array.c>

int main ( ) {
  int_array a;
  int_array_init_with ( &a, 0, 1, 2, 3, last_val );
  /*printf ( "expect {\n0\n1\n2\n3\n  }\n" );*/
  fflush ( stdout );
  int j = 0;
  for ( int_array_iter i = int_array_start ( &a ); int_array_valid ( i ); i = int_array_next ( i ) ) {
    int k = int_array_deref ( i );
    printf ( "%d == %d\n", k, j );
    fflush ( stdout );
    assert ( k == j );
    j += 1;
    }
  return 0;
  }
