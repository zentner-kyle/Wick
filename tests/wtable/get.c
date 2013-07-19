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
  const char * test_string = "this is the test string";
  wtable_wstr_ptr_to_wstr_ptr_init ( &table );
  wtable_wstr_ptr_to_wstr_ptr_set (
    &table,
    wstr_new ( test_string, strchr ( test_string, ' ' ) ),
    wstr_new ( "this value", NULL ) );

  wstr * value = NULL;
  wtable_wstr_ptr_to_wstr_ptr_get ( &table, wstr_new( "this", NULL ), &value );
  puts ( "table[\"this\"] = " );
  wstr_println ( *value );
  if ( ! value ) {
    return 1;
    }
  if ( wstr_compare ( *value, wstr_lit ( "this value" ) ) != 0 ) {
    return 1;
    }
  return 0;
  }
