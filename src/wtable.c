#include <assert.h>
#include <wtable.h>
#include <wbits.h>
#include <walloc.h>

#define SELECT_BUCKET( self, key, buckets, when_first, when_not_first, when_missing_first, when_missing_not_first ) \
  do { \
    wtable_bucket * macro_the_bucket = ( buckets ) + ( ( self->interface->hash ( key ) ) & ( self )->mask ); \
    if ( ! macro_the_bucket->hash ) { \
      when_missing_first ( macro_the_bucket ); \
      } \
    else if ( self->interface->compare ( macro_the_bucket->key, key ) == 0 ) { \
      when_first ( macro_the_bucket ); \
      } \
    else { \
      while ( macro_the_bucket->next ) { \
        macro_the_bucket = macro_the_bucket->next; \
        if ( self->interface->compare ( macro_the_bucket->key, key ) == 0 ) { \
          when_not_first ( macro_the_bucket ); \
          } \
        } \
      when_missing_not_first ( macro_the_bucket ); \
      } \
    } while ( 0 )

const size_t wtable_default_size = 8;

bool wtable_init_to_size (
    wtable * self,
    wtype * elem_type,
    size_t predicted_elems,
    wtable_elem_interface * hash_interface,
    wcall error
    ) {
  assert ( self );
  predicted_elems = round_up_power_2 ( predicted_elems );
  self->elem_type = elem_type;
  self->interface = hash_interface;
  self->mask = mask_of_pow2 ( predicted_elems );
  self->space = predicted_elems;
  self->num_elems = 0;
  self->data = walloc_static ( elem_type, error );
  return !!self->data;
  }

bool wtable_init (
    wtable * self,
    wtype * elem_type,
    wtable_elem_interface * hash_interface,
    wcall error
    ) {
  return wtable_init_to_size ( self,
                               elem_type,
                               wtable_default_size,
                               hash_interface,
                               error );
  }

void * wtable_lookup ( wtable * self, void * key ) {
  #define RETURN_VAL( bucket ) \
    return bucket->value;
  #define RETURN_NULL( bucket ) \
    return NULL;
  SELECT_BUCKET ( self,
                  key,
                  self->data,
                  RETURN_VAL,
                  RETURN_VAL,
                  RETURN_NULL,
                  RETURN_NULL );
  #undef RETURN_VAL
  #undef RETURN_NULL
  }

void * wtable_lookup_or_add (
    wtable * self,
    void * key,
    void * ( *on_missing ) ( void * key )
    ) {
  #define RETURN_FOUND( bucket ) \
    return ( bucket )->value;
  #define ADD_FIRST( bucket ) \
    ( bucket )->key = key; \
    ( bucket )->value = on_missing ( key ); \
    return ( bucket )->value;
  #define ADD_NOT_FIRST( bucket ) \
    wtable_bucket * new_bucket = walloc_simple ( wtable_bucket, 1 ); \
    ( bucket )->next = new_bucket;  \
    new_bucket->next = NULL; \
    ADD_FIRST ( new_bucket );
  SELECT_BUCKET (
      self,
      key,
      self->data,
      RETURN_FOUND,
      RETURN_FOUND,
      ADD_FIRST,
      ADD_NOT_FIRST );
  }

/* Note that these two functions share most of their implementations. */

void * wtable_lookup_default ( wtable * self, void * key, void * default_value ) {
  #undef ADD_FIRST
  #define ADD_FIRST( bucket ) \
    ( bucket )->key = key; \
    ( bucket )->value = default_value; \
    return ( bucket )->value;
  SELECT_BUCKET (
      self,
      key,
      self->data,
      RETURN_FOUND,
      RETURN_FOUND,
      ADD_FIRST,
      ADD_NOT_FIRST );
  #undef RETURN_FOUND
  }

void wtable_set ( wtable * self, void * key, void * value ) {
  #undef ADD_FIRST
  #define ADD_FIRST( bucket ) \
    ( bucket )->key = key; \
    ( bucket )->value = value;
  #define SET( bucket ) \
    ( bucket )->value = value;
  SELECT_BUCKET (
      self,
      key,
      self->data,
      SET,
      SET,
      ADD_FIRST,
      ADD_NOT_FIRST );
  #undef ADD_NOT_FIRST
  #undef SET
  }

inline bool wtable_needs_resize ( wtable * self ) {
  return self->num_elems & ( ( self->mask + 1 ) << 1 );
  }

inline void wtable_resize ( wtable * self ) {
  for ( int i = 0; i < self->space; i++ ) {
    }
  }
