#include <wtable_wval.h>
#include <wmacros.h>

#ifndef wtable_key_t
  #error Must define wtable_key_t.
  #endif

#ifndef wtable_val_t
  #error Must define wtable_val_t.
  #endif

#ifndef wtable_key_name
  #define wtable_key_name wtable_key_t
  #endif

#ifndef wtable_val_name
  #define wtable_val_name wtable_val_t
  #endif


#ifndef wtable_key_kind
  #error Please define wtable_key_kind to one of integer, uinteger, floating, or pointer.
  #endif

#ifndef wtable_val_kind
  #error Please define wtable_val_kind to one of integer, uinteger, floating, or pointer.
  #endif


#ifndef wtable_key_wtype
  #define wtable_key_wtype w_type_to_wtype ( wtable_key_name )
  #endif

#ifndef wtable_val_wtype
  #define wtable_val_wtype w_type_to_wtype ( wtable_val_name )
  #endif


#ifndef wtable_key_hash
  #define wtable_key_hash join_token ( whash_, wtable_key_name )
  #endif

#ifndef wtable_key_compare
  #define wtable_key_compare join_token ( wcompare_, wtable_key_name )
  #endif


#ifndef wtable_name
  #define wtable_name join_token ( join_token ( wtable_key_name, _to_ ), wtable_val_name )
  #endif

#define method( mname ) join_token ( join_token ( wtable_, wtable_name ), join_token ( _, mname ) )

#define wtable_struct join_token ( wtable_, wtable_name )

#define wtable_interface join_token ( wtable_, join_token ( wtable_name, _interface ) )


extern wtable_elem_interface wtable_interface;

#define wtable_struct_to_table( wtable_s ) ( &( wtable_s )->table )

#define wtable_backcast_key( key ) ((wtable_key_t) (key).wtable_key_kind)
#define wtable_backcast_val( val ) ((wtable_val_t) (val).wtable_val_kind)

#ifdef pointer
  #error pointer should not be defined
  #endif

#define pointer 0xefaced
#if wtable_key_kind == pointer
  #undef pointer
  #define wtable_cast_key( key ) ((wval) { .wtable_key_kind = ( wobj * ) key })
#else
  #undef pointer
  #define wtable_cast_key( key ) ((wval) { .wtable_key_kind = key })
  #endif

#define pointer 0xefaced
#if wtable_val_kind == pointer
  #undef pointer
  #define wtable_cast_val( val ) ((wval) { .wtable_val_kind = ( wobj * ) val })
#else
  #undef pointer
  #define wtable_cast_val( val ) ((wval) { .wtable_val_kind = val })
  #endif

#ifndef wtable_no_struct
  def_struct ( wtable_struct ) {
    wtable table;
    };
  #endif

wtable_struct * method ( new ) ( void );

werror * method ( init_to_size ) (
    wtable_struct * self,
    size_t predicted_elems );

werror * method ( init ) (
    wtable_struct * self );

werror * method ( get ) (
    wtable_struct * self,
    wtable_key_t key,
    wtable_val_t * dst );

werror * method ( get_or_add ) (
    wtable_struct * self,
    wtable_key_t key,
    wtable_val_t * dst,
    wcall * on_missing );

werror * method ( get_default ) (
    wtable_struct * self,
    wtable_key_t key,
    wtable_val_t * dst,
    wtable_val_t default_value );

werror * method ( set ) (
    wtable_struct * self,
    wtable_key_t key,
    wtable_val_t value );

werror * method ( delete ) ( wtable_struct * self );

#ifndef wtable_source
  #undef wtable_backcast_key
  #undef wtable_backcast_val
  #undef wtable_cast_key
  #undef wtable_cast_val
  #undef wtable_interface
  #undef wtable_key_compare
  #undef wtable_key_hash
  #undef wtable_key_kind
  #undef wtable_key_name
  #undef wtable_key_t
  #undef wtable_key_wtype
  #undef wtable_name
  #undef wtable_source
  #undef wtable_struct
  #undef wtable_struct_to_table
  #undef wtable_val_kind
  #undef wtable_val_name
  #undef wtable_val_t
  #undef wtable_val_wtype
  #undef method

  #endif
