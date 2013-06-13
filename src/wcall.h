#ifndef WCALL_H
#define WCALL_H
#include "wmacros.h"

typedef void (* wcall_func_t)( void * data );
typedef void * wcall_data_t;

def_struct( wcall ) {
  wcall_func_t func;
  wcall_data_t data;
};

void winvoke ( wcall c );

wcall static_wcall( wcall_func_t func, wcall_data_t data );

const wcall null_wcall;

#endif /* end of include guard: WCALL_H */
