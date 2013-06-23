#include <string.h>
#include <wcall.h>
#include <wstr.h>

#define wtable_key_t wstr *
#define wtable_key_name wstr
#define wtable_val_t wstr *
#define wtable_val_name wstr
#include <wtable.h>

int main ( ) {
  wtable_wstr_to_wstr table;
  int iterations = 1000;
  wtable_wstr_to_wstr_init ( &table, null_wcall );
  for ( long long i = 0; i < iterations; i++ ) {
    wstr * str = wstr_from_llong( i );
wtable_wstr_to_wstr_set ( &table, str, str, null_wcall );
    }

  for ( long long i = 0; i < iterations; i++ ) {
    wstr * str = wstr_from_llong( i );
    wstr * value = wtable_wstr_to_wstr_lookup ( &table, str );
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