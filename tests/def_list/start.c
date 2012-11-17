#define _LIST_T_ int
#include "def_list.c"

int main() {
  int_list * list = NULL;
  WICK_THROW(int_list_build(3, &list, 1, 2, 3));
  int_list_iterator iter;
  int_list_start(list, &iter);
  return 0;
}
