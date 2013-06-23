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


#define wdefine_ptr( base_type ) \
  typedef base_type * join_token ( base_type, _ptr ); \
  wtype_ptr join_token ( wtype_, join_token ( base_type, _ptr ) ) = { \
    &w_type_to_wtype ( wtype_ptr ), \
    wsym_lit ( string_of_macro ( join_token ( base_type, _ptr ) ) ), \
    sizeof ( base_type *), \
    (wtype *) &w_type_to_wtype ( base_type ) \
    }; \
  void wcheck_static_test ( join_token ( base_type, _ptr ) ) ( base_type * to_check ) { };

#define wdeclare_ptr( base_type ) \
  typedef base_type * join_token ( base_type, _ptr ); \
  extern wtype_ptr join_token ( wtype_, join_token ( base_type, _ptr ) ); \
  void wcheck_static_test ( join_token ( base_type, _ptr ) ) ( base_type * );

#endif /* end of include guard: WMACROS_H */
