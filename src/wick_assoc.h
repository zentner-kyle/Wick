#ifndef WICK_ASSOC_ARRAY_H
#define WICK_ASSOC_ARRAY_H
#include <stdint.h>
#include <stdbool.h>
#include "wick_dyn.h"

struct wick_place;

struct wick_assoc_cell {
	wick_dyn key;
	wick_dyn val;
};

typedef struct wick_assoc_cell wick_assoc_cell;

struct wick_assoc {
	wick_assoc_cell * start;
	uint64_t byte_length;
	uint64_t bytes_filled;
};

typedef struct wick_assoc wick_assoc;

struct wick_assoc_iter {
	wick_assoc * assoc;
	uint64_t byte_index;
};

typedef struct wick_assoc_iter wick_assoc_iter;

wick_assoc * wick_assoc_init(struct wick_place * env, wick_assoc * self);


wick_assoc_iter * wick_assoc_iter_init(struct wick_place * env, wick_assoc_iter * self, wick_assoc * assoc);

bool wick_assoc_iter_valid(struct wick_place * env, wick_assoc_iter * self);

wick_dyn wick_assoc_iter_deref(struct wick_place * env, wick_assoc_iter * self);

bool wick_assoc_iter_next(struct wick_place * env, wick_assoc_iter * self);

wick_dyn * wick_assoc_iter_key(struct wick_place * env, wick_assoc_iter * self);

wick_dyn * wick_assoc_iter_val(struct wick_place * env, wick_assoc_iter * self);


#endif /* WICK_ASSOC_ARRAY_H */
