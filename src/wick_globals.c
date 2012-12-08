#include "wick_globals.h"
#include "wick_error.h"
#include "wick_array.h"
#include "wick_dyn.h"
#include "wick_string.h"
#include "wick_array.h"
#include "wick_assoc.h"

wick_global_t wick_global = {
	/* Begin wick_core_types */
	{
		{sizeof(int)},
		{sizeof(double)},
		{sizeof(int)},
		{sizeof(wick_array)},
		{sizeof(wick_dyn)},
		{sizeof(wick_string)},
		{sizeof(wick_assoc)},
		{sizeof(wick_assoc_cell)}
	}, 
	/* End wick_core_types */
	/* Begin wick_core_errors */
	{
		WICK_SUCCESS,
		WICK_FAILURE,
		WICK_MEMORY_ERROR,
		WICK_ALLOC_ERROR,
		WICK_API_ERROR,
		WICK_STOP_ITER_ERROR,
		WICK_CONTAINER_FULL_ERROR,
		WICK_BAD_DATA,
		WICK_NULL_PTR,
		WICK_INDEX_ERROR
	}
	/* End wick_core_errors */
};
