#define _LIST_T_ int
#include "def_list.c"

int main() {
  int_list * list = NULL;
  int * i = malloc(sizeof(int));
  *i = 1337;
  WICK_THROW(int_list_push(&list, i));
  return 0;
}
