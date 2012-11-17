#include "assert.h"
#define _LIST_T_ int
#define _LIST_BIDIRECTIONAL_
#include "def_list.c"

int main() {
  int_list *list = NULL;
  WICK_THROW(int_list_build(2, &list, 1, 3));
  int_list_iterator iter; 
  int_list_start(list, &iter);
  assert(int_list_next(&iter));
  int_list_remove_prev(&list, &iter);
  assert(*int_list_deref(&iter) == 3);
  assert(!int_list_next(&iter));
  assert(!int_list_prev(&iter));
  return 0;
}
