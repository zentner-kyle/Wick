#include <wcall.h>
#include <stdlib.h>



/*
 * Should actually work like this:
 * #define WCALL_NAME to_wrap_func
 * #include <wcall_declare.h>
 *
 * ... somewhere else ...
 *
 * #define WCALL_NAME to_wrap_func
 * #define WCALL_ARGS (int) arg[0].integer, (char *) arg[1].pointer
 * #include <wcall_declare.c>
 *
 * ... somewhere else ...
 *
 * #define WCALL_NAME to_wrap_func
 * #include <wcall_init.c>
 *
 */


const int W_OK = 0;
const int W_ERROR = 1;

wstatus wcall_push ( wcall * self, wtype * type, wval val ) {
  if ( self->num_args > self->filled_args && type && type == self->arg_types[self->filled_args] ) {
    self->args[self->filled_args++] = val;
    return W_OK;
    }
  else {
    return W_ERROR;
    }
  }

wstatus wcall_pop ( wcall * self, int count ) {
  if ( self->filled_args >= count ) {
    self->filled_args -= count;
    return W_OK;
    }
  else {
    return W_ERROR;
    }
  }

wstatus wcall_set_arg_count ( wcall * self, int count ) {
  self->num_args = count;
  return W_OK;
  }

typedef wstatus (*wfunc_0) (void);
typedef wstatus (*wfunc_1) (wval arg1);
typedef wstatus (*wfunc_2) (wval arg1, wval arg2);
typedef wstatus (*wfunc_3) (wval arg1, wval arg2, wval arg3);
typedef wstatus (*wfunc_4)
  (
  wval arg1,
  wval arg2,
  wval arg3,
  wval arg4
  );
typedef wstatus (*wfunc_5)
  (
  wval arg1,
  wval arg2,
  wval arg3,
  wval arg4,
  wval arg5
  );
typedef wstatus (*wfunc_6)
  (
  wval arg1,
  wval arg2,
  wval arg3,
  wval arg4,
  wval arg5,
  wval arg6
  );
typedef wstatus (*wfunc_7)
  (
  wval arg1,
  wval arg2,
  wval arg3,
  wval arg4,
  wval arg5,
  wval arg6,
  wval arg7
  );
typedef wstatus (*wfunc_8)
  (
  wval arg1,
  wval arg2,
  wval arg3,
  wval arg4,
  wval arg5,
  wval arg6,
  wval arg7,
  wval arg8
  );
typedef wstatus (*wfunc_9)
  (
  wval arg1,
  wval arg2,
  wval arg3,
  wval arg4,
  wval arg5,
  wval arg6,
  wval arg7,
  wval arg8,
  wval arg9
  );
typedef wstatus (*wfunc_10)
  (
  wval arg1,
  wval arg2,
  wval arg3,
  wval arg4,
  wval arg5,
  wval arg6,
  wval arg7,
  wval arg8,
  wval arg9,
  wval arg10
  );

wstatus winvoke ( wcall * c ) {
  if ( c->func == NULL ) {
    return W_OK;
    }
  if ( c->num_args != c->filled_args ) {
    return W_ERROR;
    }
  switch ( c->num_args ) {
    case 0:
      return ( ( wfunc_0 ) c->func) ();
      break;
    case 1:
      return ( ( wfunc_1 ) c->func) ( c->args[0] );
      break;
    case 2:
      return ( ( wfunc_2 ) c->func) ( c->args[0], c->args[1] );
      break;
    case 3:
      return ( ( wfunc_3 ) c->func) ( c->args[0], c->args[1], c->args[2] );
      break;
    case 4:
      return ( ( wfunc_4 ) c->func) ( c->args[0],
                                      c->args[1],
                                      c->args[2],
                                      c->args[3] );
      break;
    case 5:
      return ( ( wfunc_5 ) c->func) ( c->args[0],
                                      c->args[1],
                                      c->args[2],
                                      c->args[3],
                                      c->args[4] );
      break;
    case 6:
      return ( ( wfunc_6 ) c->func) ( c->args[0],
                                      c->args[1],
                                      c->args[2],
                                      c->args[3],
                                      c->args[4],
                                      c->args[5] );
      break;
    case 7:
      return ( ( wfunc_7 ) c->func) ( c->args[0],
                                      c->args[1],
                                      c->args[2],
                                      c->args[3],
                                      c->args[4],
                                      c->args[5],
                                      c->args[6] );
      break;
    case 8:
      return ( ( wfunc_8 ) c->func) ( c->args[0],
                                      c->args[1],
                                      c->args[2],
                                      c->args[3],
                                      c->args[4],
                                      c->args[5],
                                      c->args[6],
                                      c->args[7] );
      break;
    case 9:
      return ( ( wfunc_9 ) c->func) ( c->args[0],
                                      c->args[1],
                                      c->args[2],
                                      c->args[3],
                                      c->args[4],
                                      c->args[5],
                                      c->args[6],
                                      c->args[7],
                                      c->args[8] );
      break;
    case 10:
      return ( ( wfunc_10 ) c->func) ( c->args[0],
                                       c->args[1],
                                       c->args[2],
                                       c->args[3],
                                       c->args[4],
                                       c->args[5],
                                       c->args[6],
                                       c->args[7],
                                       c->args[8],
                                       c->args[9] );
      break;
    default:
      return W_ERROR;
    }
  }

wcall null_wcall = {
  0,
  0,
  0,
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 } } };
