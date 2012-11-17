#ifndef COMMON_H

#define COMMON_H

#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <assert.h>

#define IGNORE_ERROR(_ignored_error_) (_ignored_error_)

#define WICK_SUCCESS 1

#define WICK_FAILURE 2

#define WICK_MEMORY_ERROR 3

#define WICK_ALLOC_ERROR 4

#define WICK_API_ERROR 5

#define WICK_STOP_ITER_ERROR 6

#define WICK_CONTAINER_FULL_ERROR 7

#define BAD_DATA 8

typedef uint32_t wick_error;

#define C_TOKEN_JOIN_INNER(_token_a_, _token_b_) _token_a_ ## _token_b_
#define C_TOKEN_JOIN(_token_a_, _token_b_) C_TOKEN_JOIN_INNER(_token_a_, _token_b_)

#define C_TOKEN2STRING(_token_) #_token_
#define C_MACRO2STRING(_token_) C_TOKEN2STRING(_token_)

#define WICK_THROW(_possible_error_expr_) \
	do{ \
		wick_error _possible_error_ = (_possible_error_expr_); \
		if((_possible_error_) != WICK_SUCCESS) { \
			printf ("Error in: " #_possible_error_expr_ "\n"); \
			return (_possible_error_); \
		} \
	} while (0)

#define WICK_PROPAGATE_NON_FAILURE(_func_call_) \
	do { \
		wick_error _prop_nfail_error_ = (_func_call_); \
		if(_prop_nfail_error_ != WICK_FAILURE) { \
			/*printf("Returning from call to " #_func_call_*/ \
			/*"with result %d\n", _prop_nfail_error_);*/ \
			return _prop_nfail_error_; \
			/* Could be success or an "exception". */ \
		} \
	} while (0) 

void * wick_cmalloc(size_t bytes) {
	return calloc(bytes, sizeof(uint8_t));
}

wick_error wick_host_alloc(void ** destination, size_t byte_count) {
	void * allocated = wick_cmalloc(byte_count);
	if (allocated) {
		*destination = allocated;
		return WICK_SUCCESS;
	} else {
		return WICK_ALLOC_ERROR;
	}
}

void wick_host_free(void * mem) {
	free(mem);
}

#define WICK_ALLOC(destination, byte_count) wick_host_alloc((void **) destination, byte_count)

wick_error wick_parallel_alloc(void ** item_a, size_t byte_count_a, void ** item_b, size_t byte_count_b) {
	void * alloced_a = wick_cmalloc(byte_count_a);
	void * alloced_b = wick_cmalloc(byte_count_b);
	if (!alloced_a || !alloced_b) {
		/* One allocation failed, but which one? */
		if (!alloced_a || !alloced_b) {
			/* Both failed, undo neither. */
			return WICK_ALLOC_ERROR;
		}
		if (!alloced_a) {
			/* Allocation a failed, undo b. */
			wick_host_free(alloced_b);
			return WICK_ALLOC_ERROR;
		}
		if (!alloced_b) {
			/* Allocation b failed, undo a. */
			wick_host_free(alloced_a);
			return WICK_ALLOC_ERROR;
		}
		assert(0); /* Unreachable. */
	} else {
		*item_a = alloced_a;
		*item_b = alloced_b;
		return WICK_SUCCESS;
	}
}

#define WICK_MEM_CHECK assert

#ifndef HAVE_BOOL
#define bool int
#define True 1
#define False 0
#endif

typedef uint8_t byte;

#define IGNORE_RETURN(_return_val_)	\
	do { \
		(void)(_return_val_); \
	} while (0)

#endif /* end of include guard: COMMON_H */
