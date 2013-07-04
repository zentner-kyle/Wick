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

werror * wcall_push ( wcall * self, wtype * type, wval val ) {
  if ( self->num_args > self->filled_args && type && type == self->arg_types[self->filled_args] ) {
    self->args[self->filled_args++] = val;
    return w_ok;
    }
  else {
    return &werror_generic;
    }
  }

werror * wcall_pop ( wcall * self, size_t count ) {
  if ( self->filled_args >= count ) {
    self->filled_args -= count;
    return w_ok;
    }
  else {
    return &werror_generic;
    }
  }

werror * wcall_push_types ( wcall * self, size_t count, ... ) {
  va_list types;
  va_start ( types, count );
  if ( !self || count > self->num_args ) {
    return &werror_generic;
    }
  for ( int i = 0; i < count; i++ ) {
    self->arg_types[i] = va_arg ( types, wtype * );
    }
  va_end ( types );
  return w_ok;
  }

werror * winvoke ( wcall * c ) {
  if ( c->func == NULL ) {
    return w_ok;
    }
  if ( c->num_args != c->filled_args ) {
    return &werror_generic;
    }
  else {
    return c->func ( c->args );
    }
  }

werror * wcall_clone ( wcall * dest, wcall * src ) {
  wval * new_args = walloc_simple ( wval, src->num_args );
  if ( ! new_args ) {
    return &werror_generic;
    }
  dest->func = src->func;
  dest->num_args = src->num_args;
  dest->filled_args = src->filled_args;
  dest->arg_types = src->arg_types;
  dest->args = new_args;
  memcpy ( new_args, src->args, src->filled_args * sizeof ( wval ) );
  return w_ok;
  }

werror * wcall_deinit ( wcall * c ) {
  if ( ! c ) {
    return &werror_generic;
    }
  free ( c->args );
  *c = null_wcall;
  return w_ok;
  }

werror * wcall_delete ( wcall * c ) {
  if ( ! c ) {
    return &werror_generic;
    }
  free ( c->args );
  free ( c );
  return w_ok;
  }

wcall null_wcall = {
  0,
  0,
  0,
  0,
  0,
  };
