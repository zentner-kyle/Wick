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
  int iterations = 100;
  wtable_wstr_ptr_to_wstr_ptr_init ( &table, &null_wcall );
  for ( long long i = 0; i < iterations; i++ ) {
    wstr * str = wstr_from_llong( i );
    wtable_wstr_ptr_to_wstr_ptr_set ( &table, str, str, &null_wcall );
    }
  return 0;
  }
