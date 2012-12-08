#define _LIST_T_ int
#include "def_list.h"
#include "def_list.c"

int main() {
  int_list * list = NULL;
  WICK_THROW(int_list_build(3, &list, 1, 2, 3));
  return 0;
}
