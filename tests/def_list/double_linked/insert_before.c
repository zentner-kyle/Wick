#include "assert.h"
#include "def_list.h"
#define _LIST_T_ int
#define _LIST_BIDIRECTIONAL_
#include "def_list.c"

int main() {
  int_list *list = NULL;
  WICK_THROW(int_list_build(2, &list, 1, 3));
  int_list_iterator iter; 
  int_list_start(list, &iter);
  assert(*int_list_deref(&iter) == 1);
  assert(int_list_next(&iter));
  assert(*int_list_deref(&iter) == 3);
  int i2 = 2;
  WICK_THROW(int_list_insert_before(&list, &iter, &i2));
  assert(int_list_prev(&iter));
  assert(*int_list_deref(&iter) == 2);
  assert(int_list_prev(&iter));
  assert(*int_list_deref(&iter) == 1);
  assert(!int_list_prev(&iter));
  int i0 = 0;
  WICK_THROW(int_list_insert_before(&list, &iter, &i0));
  assert(int_list_prev(&iter));
  assert(*int_list_deref(&iter) == 0);
  assert(!int_list_prev(&iter));
  list = NULL;
  int_list_start(list, &iter);
  int i3 = 3;
  WICK_THROW(int_list_insert_before(&list, &iter, &i3));
  assert(*int_list_deref(&iter) == 3);
  assert(!int_list_prev(&iter));
  assert(!int_list_next(&iter));
  return 0;
}
