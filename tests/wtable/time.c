#include <assert.h>
#include <string.h>
#include <wcall.h>
#include <wstr.h>

#define wtable_key_t wstr *
#define wtable_key_name wstr
#define wtable_val_t wstr *
#define wtable_val_name wstr
#include <wtable.h>


int main ( ) {
  char * test_string = malloc ( sizeof( "test string" ) + 1 );
  assert ( test_string );
  strcpy ( test_string, "test string" );
  wstr * str = wstr_new ( test_string, NULL );
  wtable_wstr_to_wstr table;
  int iterations = 10 * 1000;
  wtable_wstr_to_wstr_init ( &table, null_wcall );
  for ( long long i = 0; i < iterations; i++ ) {
    wtable_wstr_to_wstr_set ( &table, str, str, null_wcall );
    }
  return 0;
  }
