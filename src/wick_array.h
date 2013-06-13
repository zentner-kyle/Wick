#ifndef WICK_ARRAY_H
#define WICK_ARRAY_H
#include <stdint.h>
#include <wick_basic.h>
#include <wick_dyn.h>
#include <wick_type.h>
#include <wick_thread.h>
#include <wick_place.h>

typedef uint64_t index_t;

struct wick_array {
  wick_dyn * start;
  uint64_t byte_length;
  uint64_t bytes_filled;
  };

typedef struct wick_array wick_array;

struct wick_array_iter {
  wick_array * array;
  uint64_t byte_index;
  };

typedef struct wick_array_iter wick_array_iter;


wick_array * wick_array_init ( wick_place * place, wick_array * self );

void wick_array_append ( wick_place * place, wick_array * self, wick_dyn dynamic );

wick_dyn wick_array_pop ( wick_place * place, wick_array * self );


wick_array_iter wick_array_new_iter ( wick_place * place, wick_array * self );

wick_array_iter * wick_array_iter_init ( wick_place * env, wick_array_iter * self, wick_array * array );

bool wick_array_iter_valid ( wick_place * place, wick_array_iter * self );

wick_dyn wick_array_iter_deref ( wick_place * place, wick_array_iter * self );

bool wick_array_iter_next ( wick_place * place, wick_array_iter * self );


#endif /* WICK_ARRAY_H */
