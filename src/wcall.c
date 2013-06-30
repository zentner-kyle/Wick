#include <wcall.h>
#include <stdlib.h>
#include <stdarg.h>
#include <walloc.h>



/*
 * Should work like this:
 *
 * #define wcall_name to_wrap_func
 * #define wcall_argc 2
 * #include <wcall_declare.h>
 *
 * ... somewhere else ...
 *
 * #define wcall_name to_wrap_func
 * #define wcall_args (int) arg[0].integer, (char *) arg[1].pointer
 * #define wcall_argc 2
 * #include <wcall_define.c>
 *
 * ... somewhere else ...
 *
 * #define wcall_name to_wrap_func
 * #define wcall_wtypes wtype_int, wtype_char_ptr
 * #include <wcall_init.c>
 *
 */


wdefine_composite ( wcall );

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

wstatus wcall_pop ( wcall * self, size_t count ) {
  if ( self->filled_args >= count ) {
    self->filled_args -= count;
    return W_OK;
    }
  else {
    return W_ERROR;
    }
  }

wstatus wcall_push_types ( wcall * self, size_t count, ... ) {
  va_list types;
  va_start ( types, count );
  if ( !self || count > self->num_args ) {
    return W_ERROR;
    }
  for ( int i = 0; i < count; i++ ) {
    self->arg_types[i] = va_arg ( types, wtype * );
    }
  va_end ( types );
  return W_OK;
  }

wstatus winvoke ( wcall * c ) {
  if ( c->func == NULL ) {
    return W_OK;
    }
  if ( c->num_args != c->filled_args ) {
    return W_ERROR;
    }
  else {
    return c->func ( c->args );
    }
  }

wstatus wcall_clone ( wcall * dest, wcall * src ) {
  wval * new_args = walloc_simple ( wval, src->num_args );
  if ( ! new_args ) {
    return W_ERROR;
    }
  dest->func = src->func;
  dest->num_args = src->num_args;
  dest->filled_args = src->filled_args;
  dest->arg_types = src->arg_types;
  dest->args = new_args;
  memcpy ( new_args, src->args, src->filled_args * sizeof ( wval ) );
  return W_OK;
  }

wstatus wcall_deinit ( wcall * c ) {
  if ( ! c ) {
    return W_ERROR;
    }
  free ( c->args );
  *c = null_wcall;
  return W_OK;
  }

wstatus wcall_delete ( wcall * c ) {
  if ( ! c ) {
    return W_ERROR;
    }
  free ( c->args );
  free ( c );
  return W_OK;
  }

wcall null_wcall = {
  0,
  0,
  0,
  0,
  0,
  };
