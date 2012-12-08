#include <stdlib.h>
#include <assert.h>

#include "def_list.h"
#define _LIST_T_ int
#define _LIST_BIDIRECTIONAL_
#include "def_list.c"

int main() {
  int_list * list = NULL;
  int_list * copy = NULL;
  int_list_iterator list_iter;
  int_list_iterator copy_iter;
  bool repeat = false;
  WICK_THROW(int_list_build(3, &list, 1, 2, 3));
  WICK_THROW(int_list_copy(list, &copy));

  int_list_start(list, &list_iter);
  int_list_start(copy, &copy_iter);
  do {
    printf ("%d, %d\n", *int_list_deref(&list_iter), *int_list_deref(&copy_iter));
    fflush(stdout);
    assert(*int_list_deref(&list_iter) == *int_list_deref(&copy_iter));
    repeat = int_list_next(&list_iter);
    assert(repeat == int_list_next(&copy_iter));
  } while (repeat);

  int_list_end(list, &list_iter);
  int_list_end(copy, &copy_iter);
  do {
    printf ("%d, %d\n", *int_list_deref(&list_iter), *int_list_deref(&copy_iter));
    fflush(stdout);
    assert(*int_list_deref(&list_iter) == *int_list_deref(&copy_iter));
    repeat = int_list_prev(&list_iter);
    assert(repeat == int_list_prev(&copy_iter));
  } while (repeat);
  return 0;
}
