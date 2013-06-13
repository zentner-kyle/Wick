#include <assert.h>
#include <def_list.h>
#define list_t int
#define list_bidirectional
#include <def_list.c>

int main ( ) {
  int_list *list = NULL;
  WICK_THROW ( int_list_build ( 2, &list, 1, 3 ) );
  int_list_iterator iter; 
  int_list_start ( list, &iter );
  int_list_remove_next ( &list, &iter );
  assert ( *int_list_deref ( &iter ) == 1 );
  assert ( !int_list_next ( &iter ) );
  assert ( !int_list_prev ( &iter ) );
  return 0;
  }
