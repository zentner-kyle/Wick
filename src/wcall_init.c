#ifndef wcall_name
  #error Please define wcall_name
  #endif

wstatus wfunc_to_wcall_thunk ( wcall_name )( wval * arg ) {
  return wcall_name ( wcall_args );
  }
int wfunc_to_wcall_argc ( wcall_name ) = wcall_argc;

wfunc_to_wcall ( wcall_name ).func = &wcall_name;
wfunc_to_wcall ( wcall_name ).args = wfunc_to_wcall_args ( wcall_name );
wfunc_to_wcall ( wcall_name ).arg_types = wfunc_to_wcall_types ( wcall_name );
wfunc_to_wcall ( wcall_name ).num_args = wfunc_to_wcall_argc ( wcall_name );
wfunc_to_wcall ( wcall_name ).filled_args = 0;
wfunc_to_wcall ( wcall_name ).filled_types = 0;
#ifdef wcall_wtypes
  wcall_push_types ( &wfunc_to_wcall ( wcall_name ),
                     wfunc_to_wcall_argc ( wcall_name ),
                     wcall_wtypes );
#else
  wcall_clear_types ( &wfunc_to_wcall ( wcall_name ) );
  #endif

#undef wcall_name
#undef wcall_types
