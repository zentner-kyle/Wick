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


#define integer 1
#define uinteger 2
#define floating 3
#define pointer 4

#ifndef wtable_key_kind
  #error Please define wtable_key_kind to one of integer, uinteger, floating, or pointer.
  #endif

#ifndef wtable_val_kind
  #error Please define wtable_val_kind to one of integer, uinteger, floating, or pointer.
  #endif


#ifndef wtable_key_wtype
  #define wtable_key_wtype join_token ( wtable_key_name, _type )
  #endif

#ifndef wtable_val_wtype
  #define wtable_val_wtype join_token ( wtable_val_name, _type )
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


wtable_elem_interface wtable_interface = {
  &wtable_key_hash,
  &wtable_key_compare
  };

#define wtable_struct_to_table( wtable_s ) (&(wtable_s)->table )

//#define wtable_backcast_key( key ) ((wtable_key_t) (key).wtable_key_kind)
//#define wtable_backcast_val( val ) ((wtable_val_t) (val).wtable_val_kind)

#if wtable_key_kind == integer
  #define wtable_backcast_key( key ) ((wtable_key_t) (key).i)
#elif wtable_key_kind == uinteger
  #define wtable_backcast_key( key ) ((wtable_key_t) (key).u)
#elif wtable_key_kind == floating
  #define wtable_backcast_key( key ) ((wtable_key_t) (key).f)
#elif wtable_key_kind == pointer
  #define wtable_backcast_key( key ) ((wtable_key_t) (key).p)
  #endif

#if wtable_val_kind == integer
  #define wtable_backcast_val( val ) ((wtable_val_t) (val).i)
#elif wtable_val_kind == uinteger
  #define wtable_backcast_val( val ) ((wtable_val_t) (val).u)
#elif wtable_val_kind == floating
  #define wtable_backcast_val( val ) ((wtable_val_t) (val).f)
#elif wtable_val_kind == pointer
  #define wtable_backcast_val( val ) ((wtable_val_t) (val).p)
  #endif


#if wtable_key_kind == integer
  #define wtable_cast_key( key ) ((wval) { .i = key })
#elif wtable_key_kind == uinteger
  #define wtable_cast_key( key ) ((wval) { .u = key })
#elif wtable_key_kind == floating
  #define wtable_cast_key( key ) ((wval) { .f = key })
#elif wtable_key_kind == pointer
  #define wtable_cast_key( key ) ((wval) { .p = ( wobj * ) key })
  #endif

#if wtable_val_kind == integer
  #define wtable_cast_val( val ) ((wval) { .i = val })
#elif wtable_val_kind == uinteger
  #define wtable_cast_val( val ) ((wval) { .u = val })
#elif wtable_val_kind == floating
  #define wtable_cast_val( val ) ((wval) { .f = val })
#elif wtable_val_kind == pointer
  #define wtable_cast_val( val ) ((wval) { .p = ( wobj * ) val })
  #endif

def_struct ( wtable_struct ) {
  wtable table;
  };

bool method ( init_to_size ) (
    wtable_struct * self,
    size_t predicted_elems,
    wcall on_error );

bool method ( init ) (
    wtable_struct * self,
    wcall on_error );

wtable_val_t method ( lookup ) ( wtable_struct * self, wtable_key_t key );

wtable_val_t method ( lookup_or_add ) (
    wtable_struct * self,
    wtable_key_t key,
    wval ( *on_missing ) ( wval key ),
    wcall on_error );

wtable_val_t method ( lookup_default ) (
    wtable_struct * self,
    wtable_key_t key,
    wtable_val_t default_value,
    wcall on_error );

void method ( set ) (
    wtable_struct * self,
    wtable_key_t key,
    wtable_val_t value,
    wcall on_error );

#ifndef wtable_source
  #undef floating
  #undef integer
  #undef method
  #undef pointer
  #undef uinteger
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

  #endif
