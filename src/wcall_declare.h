#ifndef wcall_name
  #error Please define wcall_name
  #endif

#ifndef wcall_argc
  #error Please define wcall_argc
  #endif

#include <wcall.h>

wstatus wfunc_to_wcall_thunk ( wcall_name )( wval * arg );
extern int wfunc_to_wcall_argc ( wcall_name );
extern wcall wfunc_to_wcall ( wcall_name );
extern wval wfunc_to_wcall_args ( wcall_name )[wcall_argc];
extern wtype * wfunc_to_wcall_types ( wcall_name )[wcall_argc];

#undef wcall_name
#undef wcall_argc
