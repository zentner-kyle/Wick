#include "assert.h"
#include "def_list.h"
#define list_t int
#define list_bidirectional
#include "def_list.c"

int main() {
  int_list *list = NULL;
  WICK_THROW(int_list_build(2, &list, 1, 3));
  int_list_iterator iter; 
  int_list_start(list, &iter);
  assert(*int_list_deref(&iter) == 1);
  int i2 = 2;
  WICK_THROW(int_list_insert_after(&list, &iter, &i2));
  assert(int_list_move(&iter, 1));
  assert(*int_list_deref(&iter) == 2);
  assert(int_list_next(&iter));
  assert(*int_list_deref(&iter) == 3);
  assert(!int_list_next(&iter));
  return 0;
}
