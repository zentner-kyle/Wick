#include <string.h>
#include <wtable.h>
#include <wcall.h>
#include <wstr.h>

int main ( ) {
  wtable table;
  int iterations = 100;
  wtable_init ( &table, &wstr_type, &wstr_wtable_i, null_wcall );
  for ( long long i = 0; i < iterations; i++ ) {
    wstr * str = wstr_from_llong( i );
    wtable_set ( &table, ( void * ) str, ( void * ) str );
    }
  return 0;
  }
