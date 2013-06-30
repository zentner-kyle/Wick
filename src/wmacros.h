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

#define wcheck_to_wval_pointer( type, value ) ( (wval) { .pointer = ( wobj * ) wcheck_static_test ( type ) ( value ) } )
#define wcheck_to_wval_integer( type, value ) ( (wval) { .integer = wcheck_static_test ( type ) ( value ) } )
#define wcheck_to_wval_uinteger( type, value ) ( (wval) { .uinteger = wcheck_static_test ( type ) ( value ) } )
#define wcheck_to_wval_floating( type, value ) ( (wval) { .floating = wcheck_static_test ( type ) ( value ) } )

#define wdeclare_alien( alien_type ) \
  extern wtype * join_token ( wtype_, alien_type ); \
  alien_type wcheck_static_test ( alien_type ) ( alien_type to_check );

#define wdefine_alien( alien_type ) \
  wtype * join_token ( wtype_, alien_type ); \
  alien_type wcheck_static_test ( alien_type ) ( alien_type to_check ) { \
    return to_check; \
    };

#define winit_alien( alien_type ) \
  join_token ( wtype_, alien_type ) = ( wtype * ) walloc_simple ( wtype_alien, 1 ); \
  if ( join_token ( wtype_, alien_type ) == NULL ) { \
    walloc_error (); \
    } \
  join_token ( wtype_, alien_type )->type = wtype_alien; \
  join_token ( wtype_, alien_type )->name = wstr_lit ( string_of_macro ( alien_type ) ); \
  join_token ( wtype_, alien_type )->size = sizeof ( alien_type ); \
  winit_ptr ( alien_type ); \
  join_token ( wtype_, alien_type )->ptr_of = join_token ( join_token ( wtype_, alien_type ), _ptr );


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
  join_token ( wtype_, base_type ) = ( wtype * ) walloc_simple ( wtype_base, 1 ); \
  if ( join_token ( wtype_, base_type ) == NULL ) { \
    walloc_error (); \
    } \
  join_token ( wtype_, base_type )->type = wtype_wtype_base; \
  join_token ( wtype_, base_type )->name = wstr_lit ( string_of_macro ( base_type ) ); \
  join_token ( wtype_, base_type )->size = sizeof ( base_type ); \
  winit_ptr ( base_type );
  //join_token ( wtype_, base_type )->ptr_of = join_token ( join_token ( wtype_, base_type ), _ptr );


#define wdeclare_composite( composite_type ) \
  extern wtype * join_token ( wtype_, composite_type ); \
  composite_type wcheck_static_test ( composite_type ) ( composite_type to_check ); \
  wdeclare_ptr ( composite_type );

#define wdefine_composite( composite_type ) \
  wtype * join_token ( wtype_, composite_type ); \
  composite_type wcheck_static_test ( composite_type ) ( composite_type to_check ) { \
    return to_check; \
    }; \
  wdefine_ptr ( composite_type );

#define winit_composite( composite_type ) \
  join_token ( wtype_, composite_type ) = ( wtype * ) walloc_simple ( wtype_composite, 1 ); \
  if ( join_token ( wtype_, composite_type ) == NULL ) { \
    walloc_error ( ); \
    } \
  join_token ( wtype_, composite_type )->type = wtype_wtype_composite; \
  join_token ( wtype_, composite_type )->name = wstr_lit ( string_of_macro ( composite_type ) ); \
  join_token ( wtype_, composite_type )->size = sizeof ( composite_type ); \
  ( ( wtype_composite * ) join_token ( wtype_, composite_type ) )->names_to_types = wtable_wstr_ptr_to_wtype_ptr_new ( ); \
  ( ( wtype_composite * ) join_token ( wtype_, composite_type ) )->names_to_offsets = wtable_wstr_ptr_to_size_t_new ( ); \
  if ( ! ( ( wtype_composite * ) join_token ( wtype_, composite_type ) )->names_to_types || \
       ! ( ( wtype_composite * ) join_token ( wtype_, composite_type ) )->names_to_offsets ) { \
    wtable_wstr_ptr_to_wtype_ptr_delete ( ( ( wtype_composite * ) join_token ( wtype_, composite_type ) )->names_to_types ); \
    wtable_wstr_ptr_to_size_t_delete ( ( ( wtype_composite * ) join_token ( wtype_, composite_type ) )->names_to_offsets ); \
    walloc_error ( ); \
    } \
  winit_ptr ( composite_type ); \
  join_token ( wtype_, composite_type )->ptr_of = join_token ( join_token ( wtype_, composite_type ), _ptr );


#define wdeclare_ptr( base_type ) \
  typedef base_type * join_token ( base_type, _ptr ); \
  extern wtype * join_token ( wtype_, join_token ( base_type, _ptr ) ); \
  base_type * wcheck_static_test ( join_token ( base_type, _ptr ) ) ( base_type * ); \
  typedef base_type ** join_token ( base_type, _ptr_ptr ); \
  extern wtype * join_token ( wtype_, join_token ( base_type, _ptr_ptr ) ); \
  base_type ** wcheck_static_test ( join_token ( base_type, _ptr_ptr ) ) ( base_type ** );

#define wdefine_ptr( base_type ) \
  wtype * join_token ( wtype_, join_token ( base_type, _ptr ) ); \
  base_type * wcheck_static_test ( join_token ( base_type, _ptr ) ) ( base_type * to_check ) { \
    return to_check; \
    }; \
  wtype * join_token ( wtype_, join_token ( base_type, _ptr_ptr ) ); \
  base_type ** wcheck_static_test ( join_token ( base_type, _ptr_ptr ) ) ( base_type ** to_check ) { \
    return to_check; \
    };

#define winit_ptr( base_type ) \
  join_token ( join_token ( wtype_, base_type ), _ptr ) = ( wtype * ) walloc_simple ( wtype_pointer, 1 ); \
  if ( join_token ( join_token ( wtype_, base_type ), _ptr ) == NULL ) { \
    walloc_error (); \
    } \
  join_token ( join_token ( wtype_, base_type ), _ptr )->type = wtype_wtype_pointer; \
  join_token ( join_token ( wtype_, base_type ), _ptr )->name = wstr_lit ( string_of_macro ( join_token ( base_type, _ptr ) ) ); \
  join_token ( join_token ( wtype_, base_type ), _ptr )->size = sizeof ( base_type * ); \
  ( ( wtype_pointer * ) join_token ( join_token ( wtype_, base_type ), _ptr ) )->subtype = w_type_to_wtype ( base_type ); \
  join_token ( join_token ( wtype_, base_type ), _ptr_ptr ) = ( wtype * ) walloc_simple ( wtype_pointer, 1 ); \
  if ( join_token ( wtype_, base_type ) == NULL ) { \
    walloc_error (); \
    } \
  join_token ( join_token ( wtype_, base_type ), _ptr_ptr )->type = wtype_wtype_pointer; \
  join_token ( join_token ( wtype_, base_type ), _ptr_ptr )->ptr_of = NULL; \
  join_token ( join_token ( wtype_, base_type ), _ptr_ptr )->name = wstr_lit ( string_of_macro ( join_token ( base_type, _ptr_ptr ) ) ); \
  join_token ( join_token ( wtype_, base_type ), _ptr_ptr )->size = sizeof ( base_type ** ); \
  ( ( wtype_pointer * ) join_token ( wtype_, base_type ) )->subtype = w_type_to_wtype ( join_token ( base_type, _ptr ) ); \
  join_token ( join_token ( wtype_, base_type ), _ptr )->ptr_of = join_token ( join_token ( wtype_, base_type ), _ptr_ptr );

#define wobj_cast( target_type, value ) \
    ( value->type == w_type_to_wtype ( target_type ) ? ( ( target_type ) value ) : NULL )

#define wobj_of( obj ) \
    ( (void *) &( ( obj )->type ) == ( void * ) ( obj ) ? ( wobj * ) obj : NULL )

#endif /* end of include guard: WMACROS_H */
