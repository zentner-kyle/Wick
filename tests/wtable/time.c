#include <assert.h>
#include <string.h>
#include <wcall.h>
#include <wstr.h>

#define wtable_key_t wstr_ptr
#define wtable_key_kind pointer
#define wtable_val_t wstr_ptr
#define wtable_val_kind pointer
#include <wtable.h>

int main ( ) {
  char * test_string = malloc ( sizeof( "test string" ) + 1 );
  assert ( test_string );
  strcpy ( test_string, "test string" );
  wstr * str = wstr_new ( test_string, NULL );
  wtable_wstr_ptr_to_wstr_ptr table;
  int iterations = 10 * 1000;
  wtable_wstr_ptr_to_wstr_ptr_init ( &table, &null_wcall );
  for ( long long i = 0; i < iterations; i++ ) {
    wtable_wstr_ptr_to_wstr_ptr_set ( &table, str, str, &null_wcall );
    }
  return 0;
  }
