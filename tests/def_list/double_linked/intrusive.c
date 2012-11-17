#include "stdio.h"
struct int_i {
  int i;
  struct int_i * next;
  struct int_i * prev;
};

typedef struct int_i int_i;

int print_int_i(FILE *stream, int_i * i) {
  return fprintf(stream, "%d", i->i);
}


#define _LIST_T_ int_i
#define _LIST_INTRUSIVE_
#define _LIST_BIDIRECTIONAL_
#define _LIST_ITEM_PRINT_FUNC_ print_int_i
#include "def_list.c"

int main() {
  int_i * list;
  WICK_THROW(int_i_list_new(&list));
  int_i i1 = {1, 0, 0};
  int_i i2 = {2, 0, 0};
  int_i i3 = {3, 0, 0};
  WICK_THROW(int_i_list_push(&list, &i1));
  WICK_THROW(int_i_list_push(&list, &i2));
  WICK_THROW(int_i_list_push(&list, &i3));
  int_i_list_print(stdout, list);
  printf("\n");
  return 0;
}
