#include <string.h>

#include <wtable.h>
#include <wcall.h>
#include <wstr.h>

int main ( ) {
  wtable table;
  const char * test_string = "this is the test string";
  wtable_init ( &table, &wstr_type, &wstr_wtable_i, null_wcall );
  wtable_set ( &table, ( void * ) wstr_new ( test_string, strchr ( test_string, ' ' ) ), ( void * ) "this value" );
  return 0;
  }
