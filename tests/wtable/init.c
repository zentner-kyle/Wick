#include <wtable.h>
#include <wcall.h>
#include <wstr.h>

int main ( ) {
  wtable table;
  wtable_init ( &table, &wstr_type, &wstr_wtable_i, null_wcall );
  return 0;
  }
