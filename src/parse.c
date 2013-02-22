#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>

#include <stdio.h>
#include <ctype.h>

#include "wick_basic.h"

struct delegate {
	void (* func)( void * data );
	void * data;
};

typedef struct delegate delegate;

void invoke ( delegate d ) {
	assert( d.func );
	d.func( d.data );
}

uint32_t w_hash ( char * str, size_t len ) {
	const uint32_t fnv_prime = 16777619;
	switch(len) {
		case 0:
			return len;
		case 1:
			return *str;
		case 2:
			return (((*str << 3) ^ *(str+1)) + len) * fnv_prime;
		case 3:
			return fnv_prime * (((*str << 8) ^ (*(str+1) << 3) ^ *(str+2)) + len);
		default:
			return ((*(str+1) << 3) ^ len ^ (*str >> 3)) ^ 
				(fnv_prime * ((* (uint32_t *) (str + len - 4)) ^ ( * (uint16_t *) (str + (len / 2) )))) ;
	}
}

struct str {
	char * start;
	char * past_end;
};

typedef struct str str;

size_t str_len( str s ) {
	return s.past_end - s.start;
}

size_t print_str( str s ) {
	return fwrite( s.start, sizeof(char), str_len( s ), stdout );
}

size_t print_strln( str s ) {
	size_t e = fwrite( s.start, sizeof(char), str_len( s ), stdout );
	if ( e == str_len( s ) ) {
		putchar( '\n' );
		return e + 1;
	}
	return e;
}

bool str_equalp( str a, str b ) {
	return str_len( a ) == str_len( b ) && ! memcmp( a.start, b.start, str_len( a ) );
}

struct token {
	uint32_t id;
	str text;
};

typedef struct token token;

bool token_equal( token * a, token * b ) {
	assert( a && b );
	return str_equalp( a->text, b->text ) && a->id == b->id;
};

typedef uint32_t hash_t;

hash_t token_hash( token * a ) {
	return w_hash( a->text, str_len( a->text ) ) ^ a->id;
}

struct token_table {
	size_t size;
	uint32_t mask
	token ** data;
};

typedef struct token_table token_table;

const size_t token_table_starting_size = 1024;

uint32_t mask_of_pow2( uint32_t val ) {
	uint32_t result = 1;
	while ( val > 2 ) {
		result = result << 1 | result;
		val = val / 2;
	}
	return result;
}

void init_token_table( token_table * self, delegate error ) {
	assert( self );
	self->mask = mask_of_pow2( token_table_starting_size );
	self->size = token_table_starting_size;
	self->data = (token **) malloc( token_table_starting_size * sizeof( token * ) );
	if ( ! self->data ) {
		invoke(error);
	}
};

/* 
 * Finds a token in the table. If the token is not found, then a (deep) copy
 * of it is inserted. Returns the token (inserted or found).
 */
token * token_table_get( token t ) {
	hash_t 
}

const void * end_args = (void *) ~ (intptr_t)0;
/* Unaligned pointer value at end of memory space (probably in kernel space).
 * Used to indicate the last argument in a variable argument list of pointers
 * where NULL would indicate allocation failure.
 */

size_t round_up_power_2( size_t to_round ) {
	size_t next = 0;
	while ( ( next = (to_round & (to_round - 1)) ) ) {
		to_round = next;
	}
	return to_round * 2;
}

bool is_power_2( size_t input ) {
	return ! ( input & (input - 1));
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
	char * progress;
	parser_components reactors;
	parser_components lexers;
	token_array tokens;
	int error;
};

const struct {
	const int none;
	const int alloc;
} parser_errors = { 0, 1 };

void array_insert( array * a, void * elem, size_t elem_size, delegate error ) {
	assert( a && elem && error.func );
	if ( a->count == a->space ) {
		size_t new_size = a->count * 2;
		void * new_data = malloc( elem_size * new_size );
		if ( ! new_data ) {
			invoke( error );
			return;
		}
		memcpy( new_data, a->data, elem_size * a->count );
		free( a->data );
		a->data = new_data;
		a->space = new_size;
	}
	memcpy( (void *) (elem_size * a->count++ + (char *) a->data), elem, elem_size );
}

void array_init( array * self, size_t elem_size, size_t elem_count, delegate error ) {
	assert( self );
	self->count = 0;
	self->space = 0;
	self->data = malloc( elem_size * elem_count );
	if ( ! self->data ) {
		invoke( error );
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
		array_insert( (array *) &self->reactors, (void *) reactor, sizeof(parser_component), (delegate) { alloc_error, (void *) self } );
	}
}

void skip_whitespace ( parser * self ) {
	assert( self->current.start <= self->progress && self->progress < self->current.past_end );
	while ( self->progress < self->current.past_end && isspace( *self->progress ) ) {
		++self->progress;
	}
}

void init_parser( parser * self ) {
	assert( self );
	size_t num_elems = 8;
	self->error = parser_errors.none;
	delegate alloc_error_d = { .func = alloc_error, .data = (void *) self};
	array_init( (array *) &self->reactors, sizeof(parser_component), num_elems, alloc_error_d );
	array_init( (array *) &self->lexers, sizeof(parser_component), num_elems, alloc_error_d );
	array_init( (array *) &self->tokens, sizeof(token), 64, alloc_error_d );
	if ( self->error == parser_errors.alloc ) {
		array_free( (array *) &self->reactors );
		array_free( (array *) &self->lexers );
		array_free( (array *) &self->tokens );
	}
}

int main(int argv, char * argc[]) {
	printf( "%ld\n", sizeof( parser ) );
	parser p;
	init_parser( &p );
	insert_reactor( &p, &(parser_component){ &skip_whitespace, 0 } );
	/* if(argv != 2) { */
	/* 	fprintf(stderr, */
	/* 	        "Incorrect usage:\n" */
	/* 	        "program should be called\n\n" */
	/* 	        "\t%s file_name\n", argc[0]); */
	/* 	return 1; */
	/* } */
	return 0;
}
