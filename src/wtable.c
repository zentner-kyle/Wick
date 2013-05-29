#include "wtable.h"
#include "wbits.h"

const size_t wtable_default_size = 8;

bool wtable_init_to_size( wtable * self, wtype * elem_type, size_t predicted_elems, whash_interface * hash_interface, wcall error ) {
	assert( self );
	self->elem_type = elem_type;
	self->hash_interface = hash_interface;
	self->mask = mask_of_pow2( predicted_elems );
	self->space = predicted_elems;
	self->num_elems = 0;
	self->data = walloc_static( elem_type, error );
	return !!self->data;
}

bool wtable_init( token_table * self, wtype * elem_type, whash_interface * hash_interface, wcall error ) {
	return wtable_init_to_size( self, elem_type, wtable_default_size, hash_interface, error );
}

inline wtable_bucket * wtable_find_bucket( wtable * self, void * key, whash_t hashed, wtable_bucket * buckets ) {
    wtable_bucket * searcher = buckets + (hashed & self->mask);
	while ( searcher->hash ) {
		if ( searcher->hash == hashed && self->interface->compare( key, searcher->key ) == 0) {
			break;
		}
	}
	return searcher;
}

inline wtable_bucket * wtable_lookup_bucket( wtable * self, void * key, whash_t hashed ) {
	return wtable_find_bucket( self, key, hashed, self->data );
}

void * wtable_lookup( wtable * self, void * key ) {
	whash_t hashed = self->interface->hash(key);
	wtable_bucket * bucket = wtable_lookup_bucket( self, key, hashed );
	return bucket->value;
}

void * wtable_lookup_or_add( wtable * self, void * key, void * (*on_missing)(void * key) ) {
	whash_t hashed = self->interface->hash(key);
	wtable_bucket * bucket = wtable_lookup_bucket( self, key, hashed );
	if ( bucket->hash == 0 ) {
		void * value = on_missing( key );
		bucket->hash = hashed;
		bucket->key = key;
		bucket->value = value;
	}
	return bucket->value;
}

void * wtable_lookup_default( wtable * self, void * key, void * default_value ) {
	whash_t hashed = self->interface->hash(key);
	wtable_bucket * bucket = wtable_lookup_bucket( self, key, hashed );
	if ( bucket->hash == 0 ) {
		return default_value;
	}
	return bucket->value;
}

void wtable_set( wtable * self, void * key, void * value ) {
	whash_t hashed = self->interface->hash(key);
	wtable_bucket * bucket = wtable_lookup_bucket( self, key, hashed );
	bucket->value = value;
}

inline bool wtable_needs_resize( wtable * self ) {
	return self->num_elems & ((self->mask + 1) << 1);
}

inline void wtable_resize( wtable * self ) {
}
