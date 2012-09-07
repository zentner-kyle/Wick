#include <stdio.h>

#define PRINT_SIZE(type) {\
  printf("sizeof(" #type "): %lu bytes\n", sizeof(type));\
}

int main() {
  PRINT_SIZE(int);
  PRINT_SIZE(char);
  PRINT_SIZE(unsigned char);
  PRINT_SIZE(long);
  PRINT_SIZE(long long);
  PRINT_SIZE(short);
  PRINT_SIZE(char);
  PRINT_SIZE(float);
  PRINT_SIZE(double);
  PRINT_SIZE(long double);
  PRINT_SIZE(void*);
  PRINT_SIZE(size_t);
  char x = -1;
  if(x < 0) {
    printf("char is signed\n");
  } else {
    printf("char is unsigned\n");
  }
}
