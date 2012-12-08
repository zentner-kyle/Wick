##ifndef WICK_ITER_H
#define WICK_ITER_H
#include "dyn.h"

struct wick_iter {
	core_id id;
	dyn sequence;
	uint64_t index;
};

typedef struct wick_iter wick_iter;


void wick_iter_begin(wick_iter * i, dyn sequence);


#endif /* WICK_ITER_H */
