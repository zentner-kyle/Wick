#include "assert.h"
#include "def_list.h"
#define list_t int
#define list_bidirectional
#include "def_list.c"

int main() {
  int_list *list = NULL;
  WICK_THROW(int_list_build(5, &list, 1, 2, 3, 4, 5));
  int_list_iterator iter; 
  int_list_start(list, &iter);
  int_list_remove(&list, &iter);
  assert(*int_list_deref(&iter) == 2);
  assert(!int_list_prev(&iter));
  assert(int_list_next(&iter));
  assert(*int_list_deref(&iter) == 3);
  int_list_remove(&list, &iter);
  assert(*int_list_deref(&iter) == 4);
  assert(int_list_prev(&iter));
  assert(!int_list_prev(&iter));
  assert(int_list_next(&iter));
  assert(int_list_next(&iter));
  assert(!int_list_next(&iter));
  assert(*int_list_deref(&iter) == 5);
  int_list_remove(&list, &iter);
  assert(!int_list_valid_iter(list, &iter));
  int_list_start(list, &iter);
  assert(*int_list_deref(&iter) == 2);
  assert(int_list_next(&iter));
  assert(*int_list_deref(&iter) == 4);
  assert(!int_list_next(&iter));
  assert(*int_list_deref(&iter) == 4);
  assert(int_list_prev(&iter));
  assert(*int_list_deref(&iter) == 2);
  assert(!int_list_prev(&iter));
  return 0;
}
