#include <string.h>
#include <wcall.h>
#include <wstr.h>

#define wtable_key_t wstr_ptr
#define wtable_key_kind pointer
#define wtable_val_t wstr_ptr
#define wtable_val_kind pointer
#include <wtable.h>

int main ( ) {
  wtable_wstr_ptr_to_wstr_ptr table;
  int iterations = 1000;
  wtable_wstr_ptr_to_wstr_ptr_init ( &table );
  for ( long long i = 0; i < iterations; i++ ) {
    wstr * str = wstr_from_llong( i );
    wtable_wstr_ptr_to_wstr_ptr_set ( &table, str, str );
    }

  for ( long long i = 0; i < iterations; i++ ) {
    wstr * str = wstr_from_llong( i );
    wstr * value = NULL;
    wtable_wstr_ptr_to_wstr_ptr_get ( &table, str, &value );
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
