#include <stdio.h>
#include "wvm.h"
#include "walloc.c"

#define WICK_STACK_SIZE 256
#define WICK_ENV_SIZE 256

#if !defined(HAVE_COMPUTED_GOTOS) && defined(USE_COMPUTED_GOTOS)
	#error "Cannot use computed goto with this compiler."
#endif

#ifdef USE_COMPUTED_GOTOS
	#define DISPATCH goto *jump_targets[thisop & 0xff];
#else
	#define DISPATCH switch (thisop & 0xff)
#endif

static const char * wopcode_names[] = {
	#define OPCODE_LIST
	#define OPCODE_NAME
	#include "wopcodes.c"
};

static const char wopcode_args[] = {
	#define OPCODE_LIST
	#define OPCODE_ARGS
	#include "wopcodes.c"
};

void wexec_code( bytecode_t * c ) {
	bytecode_t thisop = *c;
	wasm_arg args[4];
	field_t * stack_start = walloc_simple( field_t, WICK_STACK_SIZE );
	field_t * stack_end = stack_start;
	field_t * env = walloc_simple( field_t, WICK_ENV_SIZE );
	field_t acc = 0;
	#ifdef USE_COMPUTED_GOTOS
		static const void * jump_targets[] = {
			#define OPCODE_TARGET
			#include "wopcodes.c"
		};
	#endif

	while (true) {
		DISPATCH {
			#define OPCODE_LABEL
			#define OPCODE_BODY
			#include "wopcodes.c"
		}
	}
exit:
	printf( "Exiting wexec_code\n" );
}

void print_opcodes( void ) {
	for (int i = 0; i < 0xff; i++) {
		if ( strcmp(wopcode_names[ i ], "last") == 0 ) {
			break;
		} else {
			printf( "%i, %s\n", i, wopcode_names[ i ] );
		}
	}
}

uint8_t get_opcode( wstr name ) {
	for (int i = 0; i < 0xff; i++) {
if ( strcmp(wopcode_names[ i ], "last") == 0 ) {
			break;
} else if ( wstr_compare(wstr_from_literal(wopcode_names[ i ]), name) == 0) {
			return i;
		}
	}
	return 0;
}

wstr get_opcode_name( wstr line ) {
	int i = 0;
	while ( line.text[0] == ' ' ) {
		line.text++;
		line.length--;
	}
	for (; line.text[i] != ' ' && i < line.length; i++) {
	}
	line.length = i;
	return line;
}

bytecode_t * wbytecode_from_filename( wstr filename ) {
	wstr file = wstr_from_filename( filename );
	warray * uncollapsed_code = warray_new( (wtype *)&opcode_type, null_wcall );
	wstr first_opcode_name = get_opcode_name( file );
	printf( "First opcode is: %s, %i\n", first_opcode_name.text, get_opcode(first_opcode_name) );
	return NULL;
}

int main( int argc, char *argv[] ) {
	printf( "Starting Wick VM.\n" );
	printf( "Sizeof arena = %ld, 0x%lx\n", WICK_ARENA_ALIGNMENT, WICK_ARENA_ALIGNMENT );
	print_opcodes();
	printf( "sizeof(massively_long_test_string) %ld\n", sizeof("massively_long_test_string"));
	printf( "src/test.wasm:\n%s\n", wstr_from_filename( WSTR_LIT( "src/test.wasm" ) ).text );
	wbytecode_from_filename( WSTR_LIT( "src/test.wasm" ) );
	return 0;
}
