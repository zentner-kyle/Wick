#ifndef WCALL_H
#define WCALL_H
#include <stdlib.h>

#include <wmacros.h>
#include <wtype_h.h>
#include <wval.h>
#include <wtype_h.h>

typedef int wstatus;
typedef wstatus ( * wcall_func_t ) ();
extern const int W_OK;
extern const int W_ERROR;

#define warg( type, value ) &w_type_to_wtype ( type ), wcheck_to_wval ( type, value )

def_struct ( wcall ) {
  wcall_func_t func;
  size_t num_args;
  size_t filled_args;
  wtype * arg_types[10];
  wval args[10];
  };

wstatus wcall_push ( wcall * self, wtype * type, wval val );
wstatus wcall_pop ( wcall * self, int count );
wstatus wcall_set_arg_count ( wcall * self, int count );

wstatus winvoke ( wcall * self );

extern wcall null_wcall;

#endif /* end of include guard: WCALL_H */
