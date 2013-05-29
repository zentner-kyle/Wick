#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>

#include <stdio.h>
#include <ctype.h>

#include "wick_basic.h"
#include "hash.h"
#include "wcall.h"
#include "str.h"
#include "wbits.h"

struct token {
	str text;
	uint32_t id;
};

typedef struct token token;

bool token_equal( token * a, token * b ) {
	assert( a && b );
	return a->id == b->id && str_equal( a->text, b->text );
}

hash_t token_hash( siphash_key key, token * a ) {
	return siphash_24( key, (uint8_t*) a->text.start, a->text.len );
}

token * copy_token( token t ) {
	token * new_t = (token *) malloc( sizeof( token ) );
	char * new_text = (char * ) malloc( t.text.len + 1);
	if ( ! new_t || ! new_text ) {
		free( new_t );
		free( new_text );
		return NULL;
	}

	memcpy( (void *) new_text, (void *) t.text.start, t.text.len );
	new_text[t.text.len] = '\0';
	new_t->text.start = new_text;
	new_t->text.len = t.text.len;
	new_t->id = t.id;
	return new_t;
}

struct token_table_bucket {
	hash_t hash;
	token * elem;
};

typedef struct token_table_bucket token_table_bucket;

struct token_table {
	size_t size;
	size_t fill;
	uint64_t mask;
	siphash_key hashkey;
	token_table_bucket * data;
};

typedef struct token_table token_table;

const size_t token_table_starting_size = 1024;

void token_table_init( token_table * self, wcall error ) {
	assert( self );
	self->mask = mask_of_pow2( token_table_starting_size );
	self->size = token_table_starting_size;
	self->fill = 0;
	self->data = (token_table_bucket *) malloc( token_table_starting_size * sizeof( token_table_bucket * ) );
	self->hashkey = (siphash_key){{0xf1255325d0fd78e2, 0x9644fae0e4d88426}};
	if ( ! self->data ) {
		winvoke(error);
	}
}

/* Returns true on success, false otherwise. */
bool resize_token_table( token_table * self, wcall error ) {
	size_t old_size = self->size;
	size_t new_size = self->size * 2;
	hash_t new_mask = mask_of_pow2( new_size );
	token_table_bucket * old_data = self->data;

	token_table_bucket * new_data = (token_table_bucket *) malloc( new_size );
	if ( ! new_data ) {
		winvoke( error );
		return false;
	}
	memset( new_data, 0, new_size );

	for ( int i = 0; i < old_size; i++ ) {
		int j = old_data[i].hash & new_mask;
		while ( new_data[j].hash ) {
			j = (j + 1) & new_mask;
		}
		new_data[j].hash = old_data[i].hash;
		new_data[j].elem = old_data[i].elem;
	}

	self->data = new_data;
	self->size = new_size;
	self->mask = new_mask;
	free( old_data );
	return true;
}

/* 
 * Finds a token in the table. If the token is not found, then a (deep) copy
 * of it is inserted. Returns the token (inserted or found).
 */
token * token_table_unique( token_table * self, token t, wcall error ) {
	hash_t to_find = token_hash( self->hashkey, &t );
	int i = to_find & self->mask;
	for ( ; self->data[i].hash; i = (i + 1) & self->mask ) {
		if ( self->data[i].hash == to_find ) {
			if ( token_equal( self->data[i].elem, &t ) ) {
				/* Token found. */
				return self->data[i].elem;
			}
		}
	}
	self->fill += 1;
	if ( self->fill * 10 > self->size * 8) {
		if ( ! resize_token_table( self, error ) ) {
			return NULL;
		}
		for ( i = to_find & self->mask; self->data[i].hash; i = (i + 1) & self->mask ) {
			/* Find the next free spot. */
		}
	}
	token * new_t = copy_token( t );
	if ( ! new_t ) {
		return NULL;
	}
	self->data[i].hash = to_find;
	self->data[i].elem = new_t;
	return new_t;
}

struct parser;

typedef struct parser parser;

typedef void (parser_method) ( parser * self );

struct array {
	size_t space;
	size_t count;
	void * data;
};

typedef struct array array;

struct parser_component {
	parser_method * func;
	void * data;
};

typedef struct parser_component parser_component;

struct parser_components {
	size_t space;
	size_t count;
	size_t index;
	parser_component * array;
};

typedef struct parser_components parser_components;

struct token_array {
	size_t space;
	size_t count;
	size_t index;
	token ** tokens;
};

typedef struct token_array token_array;

struct parser {
	str current;
	size_t progress;
	parser_components reactors;
	parser_components lexers;
	token_array tokens;
	int error;
};

const struct {
	const int none;
	const int alloc;
} parser_errors = { 0, 1 };

void array_insert( array * a, void * elem, size_t elem_size, wcall error ) {
	assert( a && elem && error.func );
	if ( a->count == a->space ) {
		size_t new_size = a->count * 2;
		void * new_data = malloc( elem_size * new_size );
		if ( ! new_data ) {
			winvoke( error );
			return;
		}
		memcpy( new_data, a->data, elem_size * a->count );
		free( a->data );
		a->data = new_data;
		a->space = new_size;
	}
	memcpy( (void *) (elem_size * a->count++ + (char *) a->data), elem, elem_size );
}

void array_init( array * self, size_t elem_size, size_t elem_count, wcall error ) {
	assert( self );
	self->count = 0;
	self->space = 0;
	self->data = malloc( elem_size * elem_count );
	if ( ! self->data ) {
		winvoke( error );
	}
}

void array_free( array * self ) {
	free( self->data );
}

void alloc_error( void * vparser ) {
	parser * p = (parser *) vparser;
	p->error = parser_errors.alloc;
}

void insert_reactor( parser * self, parser_component * reactor ) {
	assert( self && reactor );
	if ( ! self->error ) {
		array_insert( (array *) &self->reactors, (void *) reactor, sizeof(parser_component), (wcall) { alloc_error, (void *) self } );
	}
}

char * get_point( parser * self ) {
	return self->current.start + self->progress;
}

void skip_whitespace ( parser * self ) {
	assert( 0 <= self->progress && self->progress < self->current.len );
	while ( self->progress < self->current.len && isspace( *get_point( self ) ) ) {
		++self->progress;
	}
}

void init_parser( parser * self ) {
	assert( self );
}

int main(int argv, char * argc[]) {
	printf( "%ld\n", sizeof( parser ) );
	parser p;
	init_parser( &p );
	insert_reactor( &p, &(parser_component){ &skip_whitespace, 0 } );
	token_table table;
	wcall d = {0, 0};
	token tok1 = {str_from_c("test_token"), 0};
	token tok2 = {str_from_c("test_token"), 0};
	token_table_init(&table, d);
	token * t1 = token_table_unique( &table, tok1, d);
	token * t2 = token_table_unique( &table, tok2, d);
	printf("%d\n", t1 == t2);
	printf("%p\n", t1);
	printf("%p\n", t2);
	str_println( t2->text );
	/*printf("%s\n", t2->text.start);*/
	return 0;
}
