#include <assert.h>
#include <string.h>
#include <wtable.h>
#include <wbits.h>
#include <walloc.h>
#include <wstr.h>

void confirm_clear ( void * data, size_t byte_count ) {
  uint8_t * bytes = ( uint8_t * ) data;
  for ( int i = 0; i < byte_count; i++ ) {
    assert ( ! bytes[i] );
    }
  }

bool wtable_needs_grow ( wtable * self );

bool wtable_needs_shrink ( wtable * self );

void wtable_grow ( wtable * restrict self );

#define SELECT_BUCKET( \
    macro_self, \
    macro_key, \
    macro_buckets, \
    macro_when_first, \
    macro_when_not_first, \
    macro_when_missing_first, \
    macro_when_missing_not_first ) \
  do { \
    whash_t hashed = macro_self->interface->hash ( macro_key ); \
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

#ifdef WICK_DEBUG
  void wtable_check_buckets ( wtable * self ) {
    bool print = false;
    for ( int i = 0; i < self->space; i++ ) {
      wtable_bucket * b = self->data + i;
      if ( print ) {
        printf ( "." );
        }
      while ( b ) {
        if ( print ) {
          printf ( "," );
          }
        if ( b->hash ) {
          assert ( b->key );
          }

        /* Ensure that the next pointer is not inside the self->data. */

        if ( b->next > self->data ) {
          assert ( b->next >= self->data + self->space );
          }
        b = b->next;
        }
      }
    if ( print ) {
      printf ( "\n" );
      }
    }
#else
  #define wtable_check_buckets( ignored ) do { } while (0)
#endif

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
  self->data = walloc_simple ( wtable_bucket, predicted_elems );
  memset ( ( void * ) self->data, 0, predicted_elems * sizeof ( wtable_bucket ) );
  confirm_clear ( ( void * ) self->data, predicted_elems * sizeof ( wtable_bucket ) );
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
  wtable_check_buckets ( self );
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
    self->num_elems++; \
    ( bucket )->hash = hashed; \
    ( bucket )->key = key; \
    void * value = ( bucket )->value = on_missing ( key ); \
    wtable_check_buckets ( self ); \
    if ( wtable_needs_grow ( self ) ){ \
      wtable_grow ( self ); \
      } \
    return value;
  #define ADD_NOT_FIRST( bucket ) \
    wtable_bucket * new_bucket = walloc_simple ( wtable_bucket, 1 ); \
    ( bucket )->next = new_bucket;  \
    new_bucket->next = NULL; \
    ADD_FIRST ( new_bucket );
  wtable_check_buckets ( self );
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
    self->num_elems++; \
    ( bucket )->hash = hashed; \
    ( bucket )->key = key; \
    void * value = ( bucket )->value = default_value; \
    wtable_check_buckets ( self ); \
    if ( wtable_needs_grow ( self ) ){ \
      wtable_grow ( self ); \
      } \
    return value;
  wtable_check_buckets ( self );
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
    self->num_elems++; \
    assert ( bucket ); \
    ( bucket )->hash = hashed; \
    ( bucket )->key = key; \
    ( bucket )->value = value; \
    wtable_check_buckets ( self ); \
    if ( wtable_needs_grow ( self ) ){ \
      wtable_grow ( self ); \
      }
  #define SET( bucket ) \
    ( bucket )->value = value;
  wtable_check_buckets ( self );
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

bool wtable_needs_grow ( wtable * self ) {
  return self->num_elems >= (self->space >> 1);
  }

bool wtable_needs_shrink ( wtable * self ) {
  return self->num_elems <= (self->space >> 2);
  }

void wtable_grow ( wtable * restrict self ) {
  wtable_check_buckets ( self );
  wtable_bucket * new_buckets = walloc_simple ( wtable_bucket, self->space << 1 );
  assert ( new_buckets );
  memset ( ( void * ) new_buckets, 0, (self->space << 1) * sizeof ( wtable_bucket ) );
  confirm_clear ( ( void * ) new_buckets, (self->space << 1) * sizeof ( wtable_bucket ) );
  wtable_bucket * old_bucket = self->data;
  self->mask = (self->mask << 1) | 1;
  for ( int i = 0; i < self->space; i++ ) {
    /*fflush ( stdout );*/
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
  wtable_check_buckets ( self );
  assert ( self->num_elems == wtable_count_elems ( self ) );
  }
