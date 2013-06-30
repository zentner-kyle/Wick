#include <assert.h>
#include <string.h>
#include <wtable_wval.h>
#include <wbits.h>
#include <walloc.h>
#include <wstr.h>
#include <wval.h>

bool wtable_needs_grow ( wtable * self );

bool wtable_needs_shrink ( wtable * self );

void wtable_grow ( wtable * restrict self, wcall * on_error );

#define SELECT_BUCKET( \
    macro_self, \
    macro_key, \
    macro_hashed, \
    macro_buckets, \
    macro_when_first, \
    macro_when_not_first, \
    macro_when_missing_first, \
    macro_when_missing_not_first ) \
  do { \
    const whash hashed = macro_hashed; \
    wtable_bucket * macro_the_bucket = ( macro_buckets ) + ( ( hashed ) & ( macro_self )->mask ); \
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

size_t wtable_count_elems ( wtable * self ) {
  int out = 0;
  for ( int i = 0; i < self->space; i++ ) {
    wtable_bucket * b = self->data + i;
    while ( b ) {
      if (b->hash) {
        out += 1;
        }
      b = b->next;
      }
    }
  return out;
  }

bool wtable_init_to_size (
    wtable * self,
    wtype * key_type,
    wtype * val_type,
    size_t predicted_elems,
    wtable_elem_interface * hash_interface,
    wcall * on_error
    ) {
  assert ( self );
  predicted_elems = round_up_power_2 ( predicted_elems );
  self->key_type = key_type;
  self->val_type = val_type;
  self->interface = hash_interface;
  self->mask = mask_of_pow2 ( predicted_elems );
  self->space = predicted_elems;
  self->num_elems = 0;
  self->data = walloc_simple ( wtable_bucket, predicted_elems );
  if ( self->data == NULL ) {
    winvoke ( on_error );
    return false;
    }
  else {
    memset ( ( void * ) self->data, 0, predicted_elems * sizeof ( wtable_bucket ) );
    return true;
    }
  }

bool wtable_init (
    wtable * self,
    wtype * key_type,
    wtype * val_type,
    wtable_elem_interface * hash_interface,
    wcall * on_error
    ) {
  return wtable_init_to_size (
      self,
      key_type,
      val_type,
      wtable_default_size,
      hash_interface,
      on_error );
  }

wtable * wtable_new (
    wtype * key_type,
    wtype * val_type,
    wtable_elem_interface * hash_interface
  ) {
  wtable * self = walloc_simple ( wtable, 1 );
  if ( ! self ) {
    return NULL;
    }
  if ( wtable_init ( self,
                     key_type,
                     val_type,
                     hash_interface,
                     &null_wcall ) ) {
    return self;
    }
  else {
    free ( self );
    return NULL;
    }
  }

wval wtable_lookup ( wtable * self, wval key ) {
  return wtable_lookup_hash ( self, key, self->interface->hash ( key ) );
  }
wval wtable_lookup_hash ( wtable * self, wval key, whash hash ) {
  #define RETURN_VAL( bucket ) \
    return bucket->value;
  #define RETURN_NULL( bucket ) \
    return (wval) { .pointer = NULL };
  SELECT_BUCKET ( self,
                  key,
                  hash,
                  self->data,
                  RETURN_VAL,
                  RETURN_VAL,
                  RETURN_NULL,
                  RETURN_NULL );
  #undef RETURN_VAL
  #undef RETURN_NULL
  }

wval wtable_lookup_or_add (
    wtable * self,
    wval key,
    wcall * on_missing,
    wcall * on_error
    ) {
  return wtable_lookup_or_add_hash (
      self,
      key,
      on_missing,
      on_error,
      self->interface->hash ( key ) );
  }
wval wtable_lookup_or_add_hash (
    wtable * self,
    wval key,
    wcall * on_missing,
    wcall * on_error,
    whash hash
    ) {
  #define RETURN_FOUND( bucket ) \
    return ( bucket )->value;
  #define ADD_FIRST( bucket ) \
    self->num_elems++; \
    ( bucket )->hash = hashed; \
    ( bucket )->key = key; \
    wcall_push ( on_missing, self->key_type->ptr_of, (wval) { .pointer = ( wobj * ) &( bucket )->key } ); \
    wcall_push ( on_missing, self->val_type->ptr_of, (wval) { .pointer = ( wobj * ) &( bucket )->value } ); \
    winvoke ( on_missing ); \
    if ( wtable_needs_grow ( self ) ){ \
      wtable_grow ( self, on_error ); \
      } \
    return ( bucket )->value;
  #define ADD_NOT_FIRST( bucket ) \
    wtable_bucket * new_bucket = walloc_simple ( wtable_bucket, 1 ); \
    if ( new_bucket == NULL ) { \
      winvoke ( on_error ); \
      } \
    ( bucket )->next = new_bucket;  \
    new_bucket->next = NULL; \
    ADD_FIRST ( new_bucket );
  SELECT_BUCKET (
      self,
      key,
      hash,
      self->data,
      RETURN_FOUND,
      RETURN_FOUND,
      ADD_FIRST,
      ADD_NOT_FIRST );
  }

/* Note that these two functions share most of their implementations. */

wval wtable_lookup_default (
    wtable * self,
    wval key,
    wval default_value,
    wcall * on_error
    ) {
  return wtable_lookup_default_hash (
      self,
      key,
      default_value,
      on_error,
      self->interface->hash ( key ) );
  }

wval wtable_lookup_default_hash (
    wtable * self,
    wval key,
    wval default_value,
    wcall * on_error,
    whash hash
    ) {
  #undef ADD_FIRST
  #define ADD_FIRST( bucket ) \
    self->num_elems++; \
    ( bucket )->hash = hashed; \
    ( bucket )->key = key; \
    wval value = ( bucket )->value = default_value; \
    if ( wtable_needs_grow ( self ) ){ \
      wtable_grow ( self, on_error ); \
      } \
    return value;
  SELECT_BUCKET (
      self,
      key,
      hash,
      self->data,
      RETURN_FOUND,
      RETURN_FOUND,
      ADD_FIRST,
      ADD_NOT_FIRST );
  #undef RETURN_FOUND
  }

void wtable_set (
    wtable * self,
    wval key,
    wval value,
    wcall * on_error
    ) {
  return wtable_set_hash (
      self,
      key,
      value,
      on_error,
      self->interface->hash ( key ) );
  }
void wtable_set_hash (
    wtable * self,
    wval key,
    wval value,
    wcall * on_error,
    whash hash
    ) {
  #undef ADD_FIRST
  #define ADD_FIRST( bucket ) \
    self->num_elems++; \
    assert ( bucket ); \
    ( bucket )->hash = hashed; \
    ( bucket )->key = key; \
    ( bucket )->value = value; \
    if ( wtable_needs_grow ( self ) ){ \
      wtable_grow ( self, on_error ); \
      }
  #define SET( bucket ) \
    ( bucket )->value = value;
  SELECT_BUCKET (
      self,
      key,
      hash,
      self->data,
      SET,
      SET,
      ADD_FIRST,
      ADD_NOT_FIRST );
  #undef ADD_NOT_FIRST
  #undef SET
  }

bool wtable_needs_grow ( wtable * self ) {
  return self->num_elems >= (self->space >> 1);
  }

bool wtable_needs_shrink ( wtable * self ) {
  return self->num_elems <= (self->space >> 2);
  }

void wtable_delete ( wtable * self ) {
  if ( self ) {
    free ( self->data );
    }
  free ( self );
  }

void wtable_grow ( wtable * restrict self, wcall * on_error ) {
  wtable_bucket * new_buckets = walloc_simple ( wtable_bucket, self->space << 1 );
  if ( new_buckets == NULL ) {
    winvoke ( on_error );
    }
  memset ( ( void * ) new_buckets, 0, (self->space << 1) * sizeof ( wtable_bucket ) );
  wtable_bucket * old_bucket = self->data;
  self->mask = (self->mask << 1) | 1;
  for ( int i = 0; i < self->space; i++ ) {
    old_bucket = self->data + i;
    if ( old_bucket->hash ) {
      assert ( old_bucket->hash != 1 );
      #define IGNORE( bucket ) {}
      #define IMPOSSIBLE( bucket ) assert(0);
      #define MOVE_FIRST( bucket ) \
        memcpy ( bucket, old_bucket, sizeof ( wtable_bucket ) ); \
        ( bucket )->next = 0;
      SELECT_BUCKET (
          self,
          old_bucket->key,
          old_bucket->hash,
          new_buckets,
          IMPOSSIBLE,
          IMPOSSIBLE,
          MOVE_FIRST,
          IMPOSSIBLE );
      old_bucket = old_bucket->next;
      while ( old_bucket ) {
        #define MOVE_AND_FREE( bucket ) \
          memcpy ( bucket, old_bucket, sizeof ( wtable_bucket ) ); \
          free ( old_bucket ); \
          old_bucket = ( bucket )->next; \
          ( bucket )->next = 0;
        #define RELINK( bucket ) \
          ( bucket )->next = old_bucket; \
          old_bucket = old_bucket->next; \
          ( bucket )->next->next = 0;
        SELECT_BUCKET (
            self,
            old_bucket->key,
            old_bucket->hash,
            new_buckets,
            IMPOSSIBLE,
            IMPOSSIBLE,
            MOVE_AND_FREE,
            RELINK );
        }
      }
    }
  self->space <<= 1;
  free ( self->data );
  self->data = new_buckets;
  assert ( self->num_elems == wtable_count_elems ( self ) );
  }
