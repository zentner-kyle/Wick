#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

#include "def_list.h"

error_number int_compare_func(int *a, int *b, int32_t * result){
  *result = *a - *b;
  return WICK_SUCCESS;
}
int int_print(FILE * stream, int * i) {
  return fprintf(stream, "%d", *i);
}
#define list_t int
#define list_bidirectional
#define list_compare_func int_compare_func
#define list_item_print_func int_print
#include "def_list.c"

int main() {
  int_list * unsorted_list = NULL;
  int_list * sorted_list = NULL;
  WICK_THROW(int_list_build(5, &sorted_list, 1, 1, 2, 3, 4));
  /* WICK_THROW(int_list_build(3, &unsorted_list, 3, 2, 1)); */
  WICK_THROW(int_list_build(5, &unsorted_list, 1, 3, 4, 2, 1));
  WICK_THROW(int_list_sort(&unsorted_list));
  bool equal = false;
  WICK_THROW(int_list_equal(sorted_list, unsorted_list, &equal));
  assert(equal);
  fflush(stdout);
  return 0;
}
