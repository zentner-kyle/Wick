#include <string.h>
#include <wtable.h>
#include <wcall.h>
#include <wstr.h>

int main ( ) {
  wtable table;
  int iterations = 100 * 1000;
  wtable_init ( &table, &wstr_type, &wstr_wtable_i, null_wcall );
  for ( long long i = 0; i < iterations; i++ ) {
    wstr * str = wstr_from_llong( i );
    printf ( "setting " );
    wstr_println ( *str );
    fflush ( stdout );
    wtable_set ( &table, ( void * ) str, ( void * ) str );
    }

  for ( long long i = 0; i < iterations; i++ ) {
    wstr * str = wstr_from_llong( i );
    printf ( "getting " );
    wstr_println ( *str );
    wstr * value = ( wstr * ) wtable_lookup ( &table, str );
    if ( 0 != wstr_compare (
        *str, 
        *value ) ) {
      printf ( "wrong = " );
      wstr_println ( *value );
      return 1;
      }
    }
  return 0;
  }
