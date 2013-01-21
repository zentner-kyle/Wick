#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <stdio.h>
#include <ctype.h>

#include "wick_basic.h"

#define CTXT_T int
/* This may later be made a filemacro arg. */

#define CTXT_NAME context
#define CTXT_ARG , CTXT_T CTXT_NAME

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

struct matcher;

struct continuation {
	struct matcher * host;
	/* This struct has extra space allocated. */
};

typedef struct continuation continuation;

struct parse_result {
	int type;
	str part;
	void * continuation;
};

typedef struct parse_result parse_result;

str new_str( char * p ) {
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
} parse_result_types = {0, 1, 2, 3};

struct matcher {
	parse_result (* run)( struct matcher * vself, const str in );
	parse_result (* resume)( struct matcher * vself, continuation * con );
	bool (* insert_child)( struct matcher * vself, struct matcher * child ); /* Returns true on success. */
	/* This struct has extra space allocated. */
};

typedef struct matcher matcher;

parse_result run_matcher( matcher * m, const str in ) {
	return m->run( m, in );
}

bool insert_child ( matcher * parent, matcher * child ) {
	if ( parent && child && parent->insert_child ) {
		return parent->insert_child( parent, child );
	} else {
		return false;
	}
}

parse_result resume_match( matcher * m, continuation * con ) {
	if ( ! con || ! m || ! m->resume || con->host != m ) {
		return (parse_result){ parse_result_types.error, { 0, 0 }, 0 };
	} else {
		return m->resume( m, con );
	}
}

struct str_matcher {
	matcher base;
	str to_match;
	uint64_t len;
};

typedef struct str_matcher str_matcher;

parse_result str_matcher_method( matcher * vself, const str in ) {
	str_matcher * self = (str_matcher *) vself;
	if ( str_len( in ) < self->len ) {
		return (parse_result){parse_result_types.no, in, 0};
	}
	if ( ! memcmp( in.start, self->to_match.start, self->len ) ) {
		/* Equal strings. */
		return (parse_result){parse_result_types.yes, {in.start, in.start + self->len}, 0};
	}
	return (parse_result){parse_result_types.no, in, 0};
}

matcher * new_str_matcher( str to_match ) {
	str_matcher * m = malloc( sizeof( str_matcher ) );
	if ( m ) {
		m->len = str_len( to_match );
		m->base.run = str_matcher_method;
		m->to_match = to_match;
	}
	return (matcher *) m;
}

matcher * new_cstr_matcher( char * string_literal ) {
	return new_str_matcher( new_str( string_literal ) );
}

struct or_matcher {
	matcher base;
	size_t child_count;
	matcher ** children;
};

typedef struct or_matcher or_matcher;

struct or_continuation {
	continuation base;
	str input;
	size_t current_index;
	continuation * current_continuation;
	continuation ** child_continuations;
};

typedef struct or_continuation or_continuation;

parse_result resume_or_matcher( matcher * vself, continuation * vcon ) {
	or_continuation * con = (or_continuation *) vcon;
	or_matcher * self = (or_matcher *) vself;
	continuation * cc = con->current_continuation;
	parse_result res;
	for ( int i = con->current_index; i < self->child_count; i++ ) {
		if ( cc ) {
			res = resume_match( self->children[i], cc );
			cc = NULL;
		} else {
			res = run_matcher( self->children[i], con->input );
		}
		if ( res.type == parse_result_types.yes ) {
			or_continuation * c = (or_continuation *) malloc( sizeof( or_continuation ) );
			c->base.host = vself;
			c->current_index = i;
			c->current_continuation = res.continuation;
			res.continuation = c;
			return res;
		}
	}
	return (parse_result){parse_result_types.no, con->input, 0};
}

parse_result run_or_matcher( matcher * vself, const str in ) {
	or_continuation c;
	c.base.host = vself;
	c.input = in;
	c.current_index = 0;
	c.current_continuation = 0;
	return resume_or_matcher( vself, (continuation *) &c );
}

const void * end_args = (void *) ~ (intptr_t)0;
/* Largest possible pointer value. Should be portable to all pointer sizes, even 128 bits.
 * malloc can't put a allocation there, since there's not enough room (and it's not aligned).
 * We can't just use NULL here because an allocation could fail, returning NULL.
 * It might fail if the target has unsigned integer arithmetic where 'all bits set' is not the maximum value.
 * It might be worth noting that this is probably also in kernel memory space.
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

matcher ** matcher_va_alloc_array( size_t index, va_list lst, size_t * total ) {
	matcher * m = va_arg(lst, matcher *);
	matcher ** allocated_block = NULL;
	if ( ! m ) {
		return NULL;
	}
	if ( m == end_args ) {
		*total = index;
		/*printf( "index: %zd\n", *total);*/
		allocated_block = malloc( round_up_power_2( (sizeof( matcher * ) * index) ) );
	} else {
		allocated_block = matcher_va_alloc_array( index + 1, lst, total );
		if ( allocated_block ) {
			/*printf("writing: %p at %p", &allocated_block[index], m);*/
			allocated_block[index] = m;
		} 
		/*else {*/
			/*printf("wtf");*/
		/*}*/
	}
	return allocated_block;
}

bool or_matcher_insert_child( matcher * vself, matcher * child ) {
	or_matcher * self = (or_matcher *) vself;
	if ( is_power_2( self->child_count ) ) {
		/* Re-allocation needed. */
		/*printf( "Reallocating!\n" );*/
		matcher ** new_array = (matcher **) malloc( sizeof( matcher * ) * self->child_count * 2 );
		/*printf( "new_array = %p\n", new_array );*/
		if ( ! new_array ) {
			return false;
		} else {
			memcpy( new_array, self->children, sizeof( matcher * ) * self->child_count );
			free( self->children );
			self->children = new_array;
		}
	}
	/*printf( "Inserting!\n" );*/
	self->children[self->child_count] = child;
	++self->child_count;
	/*printf("Done inserting!\n");*/
	return true;
}

matcher * new_or_matcher ( matcher * first, ... ) {
	va_list lst;
	va_start( lst, first );
	or_matcher * m = (or_matcher *) malloc( sizeof( or_matcher ) );
	if ( m ) {
		m->children = matcher_va_alloc_array( 1, lst, &m->child_count );
		m->base.run = run_or_matcher;
		m->base.resume = resume_or_matcher;
		m->base.insert_child = or_matcher_insert_child;
	}
	if ( ! m->children ) {
		free( m );
	} else {
		m->children[0] = first;
	}
	va_end( lst );
	return (matcher *) m;
}

struct seq_matcher {
	matcher base;
	size_t child_count;
	matcher ** children;
};

typedef struct seq_matcher seq_matcher;

struct seq_continuation {
	continuation base;
	str input;
	str current_input;
};

parse_result run_seq_matcher( matcher * vself, const str in ) {
	seq_matcher * self = (seq_matcher *) vself;
	str sub_in = in;
	parse_result res;
	/*str sub_out;*/
	for (int i = 0; i < self->child_count; i++) {
		res = run_matcher( self->children[i], sub_in );
		/*printf( "seq res: %d\n", res );*/
		if ( res.type != parse_result_types.yes ) {
			return res;
		} else {
			sub_in.start = res.part.past_end;
			/*printf( "submatch: " );*/
			/*print_strln( sub_out );*/
			/*sub_in.start = sub_out.past_end;*/
		}
	}
	/*out->start = in.start;*/
	/*out->past_end = sub_out.past_end;*/
	/*printf( "out: " );*/
	/*fflush( stdout );*/
	/*print_strln( *out );*/
	return (parse_result){parse_result_types.yes, {in.start, res.part.past_end}, 0};
}

matcher * new_seq_matcher( matcher * first, ... ) {
	va_list lst;
	va_start( lst, first );
	seq_matcher * m = (seq_matcher *) malloc( sizeof( seq_matcher ) );
	if ( m ) {
		m->children = matcher_va_alloc_array( 1, lst, &m->child_count );
		m->base.run = run_seq_matcher;
	}
	if ( ! m->children ) {
		free( m );
	} else {
		m->children[0] = first;
	}
	va_end( lst );
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
	(isalnum( c ))
#include "def_char_matcher.c"

#define NAME ident_start
#define COND( c ) \
	(isalpha( c ) || c == '_')
#include "def_char_matcher.c"

#define NAME ident_inner
#define COND( c ) \
	(isalnum( c ) || c == '_')
#include "def_char_matcher.c"

matcher * new_identifier_matcher() {
	return new_seq_matcher(
			new_ident_start_matcher(),
			new_ident_inner_matcher(),
			end_args
			);
}

void print_result( parse_result res ) {
	char * r = "unknown match";
	switch ( res.type ) {
		case 0:
			r = "no";
			break;
		case 1:
			r = "yes";
			break;
		case 2:
			r = "error";
			break;
		case 3:
			r = "abort";
			break;
	}
	printf( "--------------------------------------------------------------------------------\n" );
	printf( "%s match\n", r );
	printf( "string = \"" );
	print_str( res.part );
	printf( "\"\n" );
	printf( "continuation: %p\n", res.continuation );
} 

void test_matcher( matcher * m, char * s ) {
	/*str out;*/
	print_result( run_matcher( m, new_str( s ) ) );
}

int main(int argv, char * argc[]) {
	/*printf( "%zd\n", round_up_power_2( 25 ) );*/
	matcher * greetings = new_or_matcher(
		new_cstr_matcher( "Hello" ),
		new_cstr_matcher( "Goodbye" ),
		end_args
		);
	matcher * hello_world = new_seq_matcher(
		greetings,
		new_str_matcher( new_str( " " ) ),
		new_str_matcher( new_str( "World!" ) ),
		end_args
		);
	matcher * identifier = new_identifier_matcher();
	test_matcher( new_cstr_matcher("Hello World!"), "Hello World!" );
	test_matcher( greetings, "Hello Wordl!" );
	test_matcher( hello_world, "Hello World!" );
	test_matcher( hello_world, "Goodbye World!" );
	test_matcher( identifier, "Goodbye World!" );
	test_matcher( hello_world, "Goodbye Forever World!" );
	test_matcher( hello_world, "Yo World!" );
	insert_child( greetings, new_cstr_matcher( "Yo" ) );
	test_matcher( hello_world, "Yo World!" );
	/* if(argv != 2) { */
	/* 	fprintf(stderr, */
	/* 	        "Incorrect usage:\n" */
	/* 	        "program should be called\n\n" */
	/* 	        "\t%s file_name\n", argc[0]); */
	/* 	return 1; */
	/* } */
	return 0;
}
