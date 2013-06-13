#include <wick_type.h>
#include <wick_error.h>

struct wick_core_type {
  wick_type _int;
  wick_type _double;
  wick_type _error;
  wick_type _array;
  wick_type _dyn;
  wick_type _obj;
  wick_type _string;
  wick_type _assoc;
  wick_type _assoc_cell;
  };

typedef struct wick_core_type wick_core_type;

struct wick_core_error {
  wick_error success;
  wick_error failure;
  wick_error mem;
  wick_error alloc;
  wick_error api;
  wick_error stop_iter;
  wick_error container_full;
  wick_error bad_data;
  wick_error null_ptr;
  wick_error index;
  };

typedef struct wick_core_error wick_core_error;

struct wick_global_t {
  wick_core_type type;
  wick_core_error error;
  };

typedef struct wick_global_t wick_global_t;

extern wick_global_t wick_global;
