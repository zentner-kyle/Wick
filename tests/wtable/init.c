#include <wcall.h>
#include <wstr.h>

#define wtable_key_t wstr *
#define wtable_key_name wstr
#define wtable_val_t wstr *
#define wtable_val_name wstr
#include <wtable.h>

int main ( ) {
  wtable_wstr_to_wstr table;
  wtable_wstr_to_wstr_init ( &table, null_wcall );
  return 0;
  }
