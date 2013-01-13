#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <stdio.h>
#include <ctype.h>

#include "wick_basic.h"

struct str {
	char * start;
	char * past_end;
};

typedef struct str str;

size_t str_len( str s ) {
	return s.past_end - s.start;
}

typedef int parse_result_t;

str new_str(char * p) {
	str s;
	s.start = p;
	s.past_end = p + strlen(p);
	return s;
}

struct {
	int no;
	int yes;
	int error;
	int abort;
} parse_result = {0, 1, 2, 3};

struct parse_str_and_res {
	str out;
	parse_result_t res;
};

typedef struct parse_str_and_res parse_str_and_res;

typedef parse_result_t (* matcher_method )( const str in, str * out, void * self );

struct matcher {
	matcher_method run;
	/* This struct has extra space allocated. */
};

typedef struct matcher matcher;

parse_result_t run_matcher( const str in, str * out, matcher * m ) {
	return m->run( in, out, (void *) m );
}

struct str_matcher {
	matcher_method run;
	str to_match;
	uint64_t len;
};

typedef struct str_matcher str_matcher;

parse_result_t str_matcher_method( const str in, str * out, void * vself ) {
	str_matcher * self = (str_matcher *) vself;
	if ( str_len( in ) < self->len ) {
		return parse_result.no;
	}
	if ( ! memcmp( in.start, self->to_match.start, self->len ) ) {
		/* Equal strings. */
		out->start = in.start;
		out->past_end = in.start + self->len;
		return parse_result.yes;
	}
	return parse_result.no;
}

matcher * new_str_matcher ( str to_match ) {
	str_matcher * m = malloc( sizeof( str_matcher ) );
	if ( m ) {
		m->run = str_matcher_method;
		m->to_match = to_match;
	}
	return (matcher *) m;
}

struct or_matcher {
	matcher_method run;
	size_t num_choices;
	matcher * choices;
	/* An array of matcher *'s is allocated here.
	 * choices itself is considered index 0.
	 */
};

typedef struct or_matcher or_matcher;

parse_result_t or_matcher_method( const str in, str * out, void * vself ) {
	or_matcher * self = (or_matcher *) vself;
	matcher ** m = &self->choices;
	while ( *m < self->choices + self->num_choices ) {
		parse_result_t res = run_matcher( in, out, *m );
		if ( res == parse_result.yes ) {
			return parse_result.yes;
		}
	}
	return parse_result.no;
}

const matcher * end_matchers = (matcher *) ~ (intptr_t)0;
/* Largest possible pointer value. Should be portable to all pointer sizes, even 128 bits.
 * malloc can't put a matcher there, since there's not enough room (and it's not aligned).
 * We can't just use NULL here because an allocation could fail, returning NULL.
 * It might fail if the target has unsigned integer arithmetic where 'all bits set' is not the maximum value.
 */

matcher * matcher_va_alloc_array( size_t extra_bytes, size_t index, va_list lst, size_t * total ) {
	matcher * m = va_arg(lst, matcher *);
	uint8_t * allocated_block = NULL;
	if ( ! m ) {
		return NULL;
	}
	if ( m == end_matchers ) {
		m = NULL;
		*total = index;
		allocated_block = (uint8_t *) malloc( extra_bytes + (sizeof( matcher * ) * index) );
	} else {
		allocated_block = (uint8_t *) matcher_va_alloc_array( extra_bytes, index + 1, lst, total );
	}
	if ( allocated_block ) {
		*(matcher **)(allocated_block + extra_bytes + sizeof( matcher * ) * index) = m;
		/* Sub-matchers also tend to be stored in the vararg functions that call this one. */
	}
	return (matcher *) allocated_block;
}

matcher * new_or_matcher ( matcher * first, ... ) {
	va_list lst;
	va_start( lst, first );
	size_t total = 0;
	or_matcher * m = (or_matcher *) matcher_va_alloc_array( sizeof( or_matcher ), 0, lst, & total );
	va_end( lst );
	if ( m ) {
		m->choices = first;
		m->run = or_matcher_method;
		m->num_choices = total + 1;
	}
	return (matcher *) m;
}

struct and_matcher {
	matcher_method run;
	size_t num_submatchers;
	matcher * submatchers;
	/* An array of matcher *'s is allocated here. */
	/* submatchers itself is considered index 0.  */
};

typedef struct and_matcher and_matcher;

parse_result_t and_matcher_method( const str in, str * out, void * vself ) {
	and_matcher * self = (and_matcher *) vself;
	str sub_in = in;
	str sub_out;
	matcher ** m = &self->submatchers;
	while ( *m < self->submatchers + self->num_submatchers ) {
		parse_result_t res = run_matcher( sub_in, &sub_out, *m );
		if ( res != parse_result.yes ) {
			return res;
		}
		sub_in.start = sub_out.past_end;
		++m;
	}
	out->start = in.start;
	out->past_end = sub_out.past_end;
	return parse_result.yes;
}

matcher * new_and_matcher( matcher * first, ... ) {
	va_list lst;
	va_start( lst, first );
	size_t total = 0;
	and_matcher * m = (and_matcher *) matcher_va_alloc_array( sizeof( and_matcher ), 0, lst, & total );
	va_end( lst );
	if ( m ) {
		m->submatchers = first;
		m->run = and_matcher_method;
		m->num_submatchers = total + 1;
	}
	return (matcher *) m;
}

#define NAME whitespace
#define COND( c ) \
	(c == ' ' || c == '\t')
#include "def_char_matcher.c"

#define NAME newline
#define COND( c ) \
	(c == '\n' || c == '\r')
#include "def_char_matcher.c"

#define NAME alpha
#define COND( c ) \
	(isalpha( c ))
#include "def_char_matcher.c"

#define NAME alphanum
#define COND( c ) \
	(isalphanum( c ))

int main(int argv, char * argc[]) {
	matcher * hello_world = new_and_matcher(
		new_or_matcher(
			new_str_matcher( new_str( "Hello" ) ),
			new_str_matcher( new_str( "Goodbye" ) ),
			end_matchers
			),
		new_str_matcher( new_str( " " ) ),
		new_str_matcher( new_str( "World!" ) ),
		end_matchers
		);
	str result;
	printf ("%d\n", run_matcher( new_str( "Hello World!" ), &result, hello_world ));
	printf ("%d\n", run_matcher( new_str( "Goodbye World!" ), &result, hello_world ));
	/* if(argv != 2) { */
	/* 	fprintf(stderr, */
	/* 	        "Incorrect usage:\n" */
	/* 	        "program should be called\n\n" */
	/* 	        "\t%s file_name\n", argc[0]); */
	/* 	return 1; */
	/* } */
	return 0;
}
