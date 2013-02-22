#include "assert.h"
#define list_t int
#include "def_list.h"
#include "def_list.c"

int main() {
  int_list *list = NULL;
  WICK_THROW(int_list_build(3, &list, 1, 2, 3));
  int_list_iterator iter; 
  int_list_start(list, &iter);
  assert(*int_list_deref(&iter) == 1);
  assert(int_list_move(&iter, 2));
  assert(*int_list_deref(&iter) == 3);
  assert(!int_list_move(&iter, 1));
  assert(*int_list_deref(&iter) == 3);
  return 0;
}
