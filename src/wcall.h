#ifndef WCALL_H
#define WCALL_H
#include <stdlib.h>

#include <wmacros.h>
#include <wtype_h.h>
#include <wval.h>

typedef int wstatus;
typedef wval wcall_data_t;
typedef wstatus ( * wcall_func_t ) ( wcall_data_t data );
extern const int W_OK;
extern const int W_ERROR;

#define WARG( type, value ) w_type_to_wtype ( type ), wcheck_static_type ( type, value )

def_struct ( wcall ) {
  wcall_func_t func;
  wcall_data_t data;
  size_t num_args;
  wtype * arg_types[10];
  };

wstatus winvoke_0 ( wcall * );
wstatus winvoke_1 ( wcall *, wtype, wval );

wcall static_wcall ( wcall_func_t func, wcall_data_t data );

extern wcall null_wcall;

#endif /* end of include guard: WCALL_H */
