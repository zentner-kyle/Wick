#ifndef SYMBOL_H

#define SYMBOL_H

#include "object.h"

struct symbol;
typedef struct symbol symbol;

struct symbol {
	object header;
	uint64_t hash;
	str text;
}

#endif /* end of include guard: SYMBOL_H */
