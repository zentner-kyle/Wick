#include <assert.h>
#include <string.h>
#include <wtable.h>
#include <wcall.h>
#include <wstr.h>

int main ( ) {
  char * test_string = malloc ( sizeof( "test string" ) + 1 );
  assert ( test_string );
  strcpy ( test_string, "test string" );
  wstr * str = wstr_new ( test_string, NULL );
  wtable table;
  int iterations = 10 * 1000;
  wtable_init ( &table, &wstr_type, &wstr_wtable_i, null_wcall );
  for ( long long i = 0; i < iterations; i++ ) {
    wtable_set ( &table, ( void * ) str, ( void * ) str );
    }
  return 0;
  }
