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
		if ( strcmp( wopcode_names[ i ], "last") == 0 ) {
			break;
		} else {
			printf( "%i, %s\n", i, wopcode_names[ i ] );
		}
	}
}

uint8_t get_opcode( wstr name ) {
	for (int i = 0; i < 0xff; i++) {
if ( strcmp( wopcode_names[ i ], "last" ) == 0 ) {
			break;
} else if ( wstr_compare( wstr_from_literal( wopcode_names[ i ] ), name ) == 0 ) {
			return i;
		}
	}
	return 0;
}

void get_opcode_name( wstr line, wstr * name, wstr * rest ) {
	int i = 0;
	while ( line.text[0] == ' ' ) {
		line.text++;
		line.length--;
	}
	for (; line.text[i] != ' ' && i < line.length; i++) {}
	wstr_init_dynamic( name );
	wstr_init_dynamic( rest );
	name->text = line.text;
	name->length = i;
	rest->text = line.text + name->length;
	rest->length = line.length - name->length;
}

void get_next_line( wstr file, wstr * line, wstr * rest ) {
	int i = 0;
	for (; file.text[i] != '\n' && i < file.length; i++) {}
	wstr_init_dynamic( line );
	wstr_init_dynamic( rest );
	if (file.text[i] == '\n') {
		line->text = file.text;
		line->length = i;
		rest->text = file.text + line->length + 1;
		rest->length = file.length - (line->length + 1);
	} else {
		line->text = file.text;
		line->length = i;
		rest->text = file.text + line->length;
		rest->length = file.length - line->length;
	}
}

uint32_t get_arg( wstr line, wstr * rest ) {
	/* TODO */
	return 0;
}

bytecode_t * wbytecode_from_filename( wstr filename ) {
	wstr file = wstr_from_filename( filename );
	wstr file_remaining = file;
	wstr line;
	warray * uncollapsed_code = warray_new( (wtype *)&opcode_type, null_wcall );
	/* wstr first_opcode_name = get_opcode_name( file ); */
	/* printf( "First opcode is: %s, %i\n", first_opcode_name.text, get_opcode( first_opcode_name ) ); */
	while (file_remaining.length != 0) {
		get_next_line( file_remaining, &line, &file_remaining );
		printf( "Line: " );
		wstr_println( line );
		wstr name;
		get_opcode_name(line, &name, &line);
		printf( "Opcode: " );
		wstr_println( name );
		uint32_t opcode = get_opcode( name );
		uint8_t size = wopcode_args[opcode];
		for (int i = 1; i <= size; i++) {
			opcode &= get_arg(line, &line) << (i * 8);
		}
		printf( "Opcode index = %zd\n", opcode );
		warray_push_back( uncollapsed_code, (void *) &opcode, null_wcall );
	}
	return NULL;
}

int main( int argc, char *argv[] ) {
	printf( "Starting Wick VM.\n" );
	/* printf( "Sizeof arena = %ld, 0x%lx\n", WICK_ARENA_ALIGNMENT, WICK_ARENA_ALIGNMENT ); */
	/* print_opcodes(); */
	/* printf( "sizeof(massively_long_test_string) %ld\n", sizeof("massively_long_test_string")); */
	/* printf( "src/test.wasm:\n%s\n", wstr_from_filename( WSTR_LIT( "src/test.wasm" ) ).text ); */
	wbytecode_from_filename( WSTR_LIT( "src/test.wasm" ) );
	return 0;
}
