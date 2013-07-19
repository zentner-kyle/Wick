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
  return 0;
  }
