
#include <string.h>
#include <wtable.h>
#include <wcall.h>
#include <wstr.h>

int main ( ) {
  wtable table;
  const char * test_string = "this is the test string";
  wtable_init ( &table, &wstr_type, &wstr_wtable_i, null_wcall );
  wtable_set (
      &table,
      ( void * ) wstr_new ( test_string, strchr ( test_string, ' ' ) ),
      ( void * ) "this value" );

  wtable_set (
      &table,
      ( void * ) wstr_new ( test_string, strchr ( test_string, ' ' ) ),
      ( void * ) "this value 2" );

  char * value = wtable_lookup ( &table, wstr_new( "this", NULL ) );
  printf( "table[\"this\"] = %s\n", value );
  if ( ! value ) {
    return 1;
    }
  if ( strcmp ( value, "this value 2" ) != 0 ) {
    return 1;
    }
  return 0;
  }
