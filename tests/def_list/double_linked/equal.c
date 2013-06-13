#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <def_list.h>

error_number int_compare_func ( int *a, int *b, int32_t * result ) {
  *result = *a - *b;
  return WICK_SUCCESS;
  }
int int_print ( FILE * stream, int * i ) {
  return fprintf ( stream, "%d", *i );
  }
#define list_t int
#define list_bidirectional
#define list_compare_func int_compare_func
#define list_item_print_func int_print
#include <def_list.c>

int main ( ) {
  int_list * list1 = NULL;
  int_list * list2 = NULL;
  WICK_THROW ( int_list_build ( 5, &list1, 1, 1, 2, 3, 4 ) );
  WICK_THROW ( int_list_build ( 5, &list2, 1, 1, 2, 3, 4 ) );
  bool equal = false;
  WICK_THROW ( int_list_equal ( list1, list2, &equal ) );
  assert ( equal );
  return 0;
  }
