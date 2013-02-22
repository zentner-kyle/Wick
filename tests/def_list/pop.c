#include "assert.h"
#include "def_list.h"
#define list_t int
#include "def_list.c"

int main() {
  int_list *list = NULL;
  int * i = malloc(sizeof(int));
  *i = 1337;
  WICK_THROW(int_list_push(&list, i));
  int * i2;
  int_list_pop(&list, &i2);
  assert(i != i2); /* Ensure that the list is copying. */
  assert(*i2 == 1337); /* Ensure that the value is stored correctly. */
  return 0;
}
