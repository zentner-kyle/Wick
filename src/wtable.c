#include <assert.h>
#include <string.h>
#include <wtable.h>
#include <wbits.h>
#include <walloc.h>

#define SELECT_BUCKET( \
    macro_self, \
    macro_key, \
    macro_buckets, \
    macro_when_first, \
    macro_when_not_first, \
    macro_when_missing_first, \
    macro_when_missing_not_first ) \
  do { \
    wtable_bucket * macro_the_bucket = ( macro_buckets ) + ( ( macro_self->interface->hash ( macro_key ) ) & ( macro_self )->mask ); \
    if ( ! macro_the_bucket->hash ) { \
      macro_when_missing_first ( macro_the_bucket ); \
      } \
    else if ( macro_self->interface->compare ( macro_the_bucket->key, macro_key ) == 0 ) { \
      macro_when_first ( macro_the_bucket ); \
      } \
    else { \
      while ( macro_the_bucket->next ) { \
        macro_the_bucket = macro_the_bucket->next; \
        if ( macro_self->interface->compare ( macro_the_bucket->key, macro_key ) == 0 ) { \
          macro_when_not_first ( macro_the_bucket ); \
          } \
        } \
      macro_when_missing_not_first ( macro_the_bucket ); \
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

inline bool wtable_needs_grow ( wtable * self ) {
  /* self->num_elems / self->space >= 0.5 */
  return self->num_elems >= (self->space >> 1);
  }

inline bool wtable_needs_shrink ( wtable * self ) {
  return self->num_elems <= (self->space >> 2);
  }

inline void wtable_resize ( wtable * self ) {
  wtable_bucket * new_buckets = walloc_simple ( wtable_bucket, self->space << 1 );
  wtable_bucket * old_bucket = self->data;
  self->mask = (self->mask << 1) | 1;
  #define IGNORE( bucket ) {}
  #define IMPOSSIBLE( bucket ) assert(0);
  #define MOVE_FIRST( bucket ) \
    memcpy ( bucket, old_bucket, sizeof ( wtable_bucket ) );
  #define MOVE_AND_FREE( bucket ) \
    memcpy ( bucket, old_bucket, sizeof ( wtable_bucket ) ); \
    free ( old_bucket );
  #define RELINK( bucket ) \
    bucket->next = old_bucket;
  for ( int i = 0; i < self->space; i++ ) {
    old_bucket = self->data + i;
    if (old_bucket->hash) {
      SELECT_BUCKET (
          self,
          old_bucket->key,
          new_buckets,
          IMPOSSIBLE,
          IMPOSSIBLE,
          MOVE_FIRST,
          IMPOSSIBLE );
      old_bucket = old_bucket->next;
      while (old_bucket) {
        SELECT_BUCKET (
            self,
            old_bucket->key,
            new_buckets,
            IMPOSSIBLE,
            IMPOSSIBLE,
            MOVE_AND_FREE,
            RELINK );
        old_bucket = old_bucket->next;
        }
      }
    }
  free ( self->data );
  self->data = new_buckets;
  }
