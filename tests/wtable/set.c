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
  wtable_wstr_to_wstr_init ( &table, null_wcall );
  wtable_wstr_to_wstr_set (
    &table,
    wstr_new ( test_string, strchr ( test_string, ' ' ) ),
    wstr_new ( "this value", NULL ),
    null_wcall );
  return 0;
  }
