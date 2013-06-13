#include <assert.h>
#include <def_list.h>
#define list_t int
#define list_bidirectional
#include <def_list.c>

int main ( ) {
  int_list * list = NULL;
  int * i = malloc ( sizeof ( int ) );
  *i = 1337;
  WICK_THROW ( int_list_push ( &list, i ) );
  assert ( !list->prev );
  assert ( !list->next );
  return 0;
  }
