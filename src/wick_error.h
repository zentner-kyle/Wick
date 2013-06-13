#ifndef WICK_ERROR_H
#define WICK_ERROR_H
#include <stdint.h>

struct wick_place;

#define WICK_SUCCESS {1}

#define WICK_FAILURE {2}

#define WICK_MEMORY_ERROR {3}

#define WICK_ALLOC_ERROR {4}

#define WICK_API_ERROR {5}

#define WICK_STOP_ITER_ERROR {6}

#define WICK_CONTAINER_FULL_ERROR {7}

#define WICK_BAD_DATA {8}

#define WICK_NULL_PTR {9}

#define WICK_INDEX_ERROR {10}

struct wick_error {
  uint32_t error_num;
};

typedef struct wick_error wick_error;

void wick_signal_alloc_error(struct wick_place * place);

#endif /* WICK_ERROR_H */
