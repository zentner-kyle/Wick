#ifndef WCALL_H
#define WCALL_H
#include <stdlib.h>
#include <string.h>

#include <wmacros.h>
#include <wtype_h.h>
#include <wval.h>
#include <wtype_h.h>
#include <werror.h>

typedef werror * ( * wcall_func_t ) (wval * arg);
extern werror * w_ok;
extern werror werror_generic;

#define warg( type, kind, value ) w_type_to_wtype ( type ), wcheck_to_wval_##kind ( type, value )

def_struct ( wcall ) {
  wval * args;
  wcall_func_t func;
  size_t num_args;
  size_t filled_args;
  wtype ** arg_types;
  };

wdeclare_composite ( wcall );

werror * wcall_push ( wcall * self, wtype * type, wval val );
werror * wcall_pop ( wcall * self, size_t count );

werror * wcall_push_types ( wcall * self, size_t count, ... );

werror * winvoke ( wcall * self );

werror * wcall_clone ( wcall * dest, wcall * src );
werror * wcall_deinit ( wcall * c );
werror * wcall_delete ( wcall * c );

extern wcall null_wcall;

#define wfunc_to_wcall_thunk( func_name ) \
  ( join_token ( wcall_thunk_, func_name ) )

#define wfunc_to_wcall( func_name ) \
  ( join_token ( wcall_, func_name ) )

#define wfunc_to_wcall_argc( func_name ) \
  ( join_token ( wcall_argc_, func_name ) )

#define wfunc_to_wcall_args( func_name ) \
  ( join_token ( wcall_args_, func_name ) )

#define wfunc_to_wcall_types( func_name ) \
  ( join_token ( wcall_types_, func_name ) )


#endif /* end of include guard: WCALL_H */
