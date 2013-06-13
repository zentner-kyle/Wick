#ifndef WICK_TYPE_H
#define WICK_TYPE_H
#include <stdlib.h>

struct wick_dyn;

struct wick_type {
  size_t size;
};

typedef struct wick_type wick_type;

void wick_type_init(wick_type * type, size_t size);

#endif /* WICK_TYPE_H */
