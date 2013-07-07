#ifndef WOBJ_H
#define WOBJ_H
#include <wmacros.h>

def_struct ( wobj ) {
  wtype * type;
  };

wdeclare_composite ( wobj );

#define wobj_to( target_type, store_ptr, expr ) \
  do { \
    wobj * macro_the_expr = (expr); \
    target_type ** macro_the_store_ptr = &(store_ptr); \
    if ( wobj->type == target_type##_type ) { \
      *macro_the_store_ptr = ( target_type * ) wobj; \
      } \
    else { \
      *macro_the_store_ptr = NULL; \
      WOBJ_CAST_ERROR ( target_type, store_ptr, macro_the_expr ); \
      } \
    } while ( 0 )

#define wobj_expr( target_type, varname, expr ) \
  do { \
    target_type * varname; \
    wobj_to( target_type, varname, expr ); \
    } while ( 0 )

#endif /* WOBJ_H */
