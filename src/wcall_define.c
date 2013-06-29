#ifndef wcall_name
  #error Please define wcall_name
  #endif

#ifndef wcall_args
  #error Please define wcall_args
  #endif

#ifndef wcall_argc
  #error Please define wcall_argc
  #endif

#include <wcall.h>

wstatus wfunc_to_wcall_thunk ( wcall_name )( wval * arg ) {
  return wcall_name ( wcall_args );
  }
int wfunc_to_wcall_argc ( wcall_name ) = wcall_argc;
wcall wfunc_to_wcall ( wcall_name );
wval wfunc_to_wcall_args ( wcall_name )[wcall_argc];
wtype * wfunc_to_wcall_types ( wcall_name )[wcall_argc];

#undef wcall_name
#undef wcall_args
#undef wcall_argc
