#ifndef WTABLE_H
#define WTABLE_H
#include <stdbool.h>
#include "wmacros.h"
#include "wtype_h.h"
#include "whash.h"

def_struct( wtable_elem_interface ) {
	whash_func_t * hash;
	whash_compare_func_t * compare;
};

def_struct( wtable_bucket ) {
	whash_t hash;
	void * key;
	void * value;
};

def_struct( wtable ) {
	wtype * type;
    wtable_elem_interface * interface;
	size_t mask;
	size_t num_elems;
	wtable_bucket * data;
};

/* Badly need to figure out dynamic interfaces in static code.
 * Porbably should build most of it on top of wcalls or similar.
 */

bool wtable_init_to_size( wtable * self, wtype * elem_type, size_t predicted_elems, wcall error );

bool wtable_init( wtable * self, wtype * elem_type, wtable_elem_interface * hash_interface, wcall error );

inline wtable_bucket * wtable_find_bucket( wtable * self, void * key, whash_t hashed, wtable_bucket * buckets );

inline wtable_bucket * wtable_lookup_bucket( wtable * self, void * key, whash_t hashed );

void * wtable_lookup( wtable * self, void * key );

void * wtable_lookup_or_add( wtable * self, void * key, void * (*on_missing)(void * key) );

void * wtable_lookup_default( wtable * self, void * key, void * default_value );

void wtable_set( wtable * self, void * key, void * value );


#endif /* end of include guard: WTABLE_H */
