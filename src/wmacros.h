#ifndef WMACROS_H
#define WMACROS_H


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
#define tuple2_1( t ) tuple2_1_inner ( t )
#define tuple2_1_inner( a, b ) a

#define tuple2_2( t ) tuple2_2_inner ( t )
#define tuple2_2_inner( a, b ) b

#define w_type_to_wtype( type ) ( join_token ( wtype_, type ) )

#define wcheck_static_test( type ) ( join_token ( wcheck_, type ) )
#define wcheck_static_type( type, value ) (1 ? ( value ) : wcheck_static_test ( type ) ( value ) )


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


#endif /* end of include guard: WMACROS_H */
