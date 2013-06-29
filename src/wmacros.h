#ifndef WMACROS_H
#define WMACROS_H

#define wdbg_prnt( format, thing ) \
  printf ( #thing " == " format "\n", (thing) );

/* Structs and typedefs. */
#define declare_struct( name ) \
  struct name; \
  typedef struct name name;

#define def_struct( name ) \
  struct name; \
  typedef struct name name; \
  struct name

#define def_union( name ) \
  union name; \
  typedef union name name; \
  union name


/* Token manipulation. */
#define join_token( a, b ) join_token_inner ( a, b )
#define join_token_inner( a, b ) a ## b

#define string_of_macro( m ) string_of_token ( m )
#define string_of_token( t ) #t


/* Tuple manipulation. */

#define w_type_to_wtype( type ) ( join_token ( wtype_, type ) )

#define wcheck_static_test( type ) ( join_token ( wcheck_, type ) )
#define wcheck_static_type( type, value ) (1 ? ( value ) : wcheck_static_test ( type ) ( value ) )
#define wcheck_to_wval( type, value ) (1 ? ( ( wval ) { value } ) : wcheck_static_test ( type ) ( value ) )


#define wdeclare_alien( alien_type ) \
  extern wtype * join_token ( wtype_, alien_type ); \
  alien_type wcheck_static_test ( alien_type ) ( alien_type to_check );

#define wdefine_alien( alien_type ) \
  wtype * join_token ( wtype_, alien_type ); \
  alien_type wcheck_static_test ( alien_type ) ( alien_type to_check ) { \
    return to_check; \
    };

#define winit_alien( alien_type ) \
  join_token ( wtype_, alien_type ) = ( wtype * ) & ( wtype_alien ) { \
    ( wtype * ) &w_type_to_wtype ( wtype_alien ), \
    wsym_lit ( string_of_macro ( alien_type ) ), \
    sizeof ( alien_type *), \
    };


#define wdeclare_base( base_type ) \
  extern wtype * join_token ( wtype_, base_type ); \
  base_type wcheck_static_test ( base_type ) ( base_type to_check ); \
  wdeclare_ptr ( base_type );

#define wdefine_base( base_type ) \
  wtype * join_token ( wtype_, base_type ); \
  base_type wcheck_static_test ( base_type ) ( base_type to_check ) { \
    return to_check; \
    }; \
  wdefine_ptr ( base_type );

#define winit_base( base_type ) \
  join_token ( wtype_, base_type ) = ( wtype * ) & ( wtype_base ) { \
    ( wtype * ) &w_type_to_wtype ( wtype_base ), \
    wsym_lit ( string_of_macro ( base_type ) ), \
    sizeof ( base_type *), \
    }; \
  winit_ptr ( base_type );


#define wdeclare_ptr( base_type ) \
  typedef base_type * join_token ( base_type, _ptr ); \
  extern wtype * join_token ( wtype_, join_token ( base_type, _ptr ) ); \
  base_type * wcheck_static_test ( join_token ( base_type, _ptr ) ) ( base_type * );

#define wdefine_ptr( base_type ) \
  wtype * join_token ( wtype_, join_token ( base_type, _ptr ) ); \
  base_type * wcheck_static_test ( join_token ( base_type, _ptr ) ) ( base_type * to_check ) { \
    return to_check; \
    };

#define winit_ptr( base_type ) \
  join_token ( wtype_, join_token ( base_type, _ptr ) ) = (wtype *) & ( wtype_ptr ) { \
    w_type_to_wtype ( wtype_ptr ), \
    wsym_lit ( string_of_macro ( join_token ( base_type, _ptr ) ) ), \
    sizeof ( base_type *), \
    (wtype *) w_type_to_wtype ( base_type ) \
    };

#define Wfunc_to_wcall_thunk( func_name ) \
  ( join_token ( wcall_thunk_, func_name ) )

#define wfunc_to_wcall( func_name ) \
  ( join_token ( wcall_, func_name ) )

#define wfunc_to_wcall_argc( func_name ) \
  ( join_token ( wcall_argc_, func_name ) )

#define wfunc_to_wcall_args( func_name ) \
  ( join_token ( wcall_args_, func_name ) )

#define wfunc_to_wcall_types( func_name ) \
  ( join_token ( wcall_types_, func_name ) )



#endif /* end of include guard: WMACROS_H */
