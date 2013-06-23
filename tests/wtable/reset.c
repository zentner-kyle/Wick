#include <string.h>
#include <wcall.h>
#include <wstr.h>

#define wtable_key_t wstr *
#define wtable_key_name wstr
#define wtable_key_kind pointer
#define wtable_val_t wstr *
#define wtable_val_name wstr
#define wtable_val_kind pointer
#include <wtable.h>


int main ( ) {
  wtable_wstr_to_wstr table;
  const char * test_string = "this is the test string";
  wtable_wstr_to_wstr_init ( &table, &null_wcall );
  wtable_wstr_to_wstr_set (
      &table,
      wstr_new ( test_string, strchr ( test_string, ' ' ) ),
      wstr_new ( "this value", NULL ),
      &null_wcall );

  wtable_wstr_to_wstr_set (
      &table,
      wstr_new ( test_string, strchr ( test_string, ' ' ) ),
      wstr_new ( "this value 2", NULL),
      &null_wcall );

  wstr * value = wtable_wstr_to_wstr_lookup ( &table, wstr_new( "this", NULL ) );
  puts ( "table[\"this\"] = " );
  wstr_println ( *value );
  if ( ! value ) {
    return 1;
    }
  if ( wstr_compare ( *value, wstr_lit ( "this value 2" ) ) != 0 ) {
    return 1;
    }
  return 0;
  }
