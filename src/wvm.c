#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "wvm.h"
#include "walloc.h"
#include "werror.h"

#define WICK_STACK_SIZE 256
#define WICK_ENV_SIZE 256

/* #undef USE_COMPUTED_GOTOS */

#if !defined(HAVE_COMPUTED_GOTOS) && defined(USE_COMPUTED_GOTOS)
	#error "Cannot use computed goto with this compiler."
#endif

#ifdef USE_COMPUTED_GOTOS
	#define DISPATCH goto *jump_targets[op_bunch & 0xff];
#else
	#define DISPATCH switch (op_bunch & 0xff)
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
	bytecode_t op_bunch = *c;
	wasm_arg args[4];
	field_t * stack_start = walloc_simple( field_t, WICK_STACK_SIZE );
	field_t * stack_end = stack_start;
	field_t * env = walloc_simple( field_t, WICK_ENV_SIZE );
	field_t acc = 0;
#define get_arg( a ) \
(stack_start + a)
	/* #define get_arg( a )																			\ */
	/* 	(( a & ( 1 << 7 ) ) ? (stack_end + a) : (stack_start + a)) */
	#define arg_0 (*get_arg(args[0]))
	#define arg_1 (*get_arg(args[1]))
	#define arg_2 (*get_arg(args[2]))
/* printf("Here\n"); */

	#ifdef USE_COMPUTED_GOTOS
		static const void * jump_targets[] = {
			#define OPCODE_TARGET
			#include "wopcodes.c"
		};
	#endif

	while (op_bunch) {
/* printf("."); */
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

void skip_wspace( wstr * line ) {
	while ( (isspace( line->text[ 0 ] ) || line->text[ 0 ] == '\n') && line->length != 0 ) {
		++line->text;
		--line->length;
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
	skip_wspace( &line );
	for (; ! isspace( line.text[i] ) && i < line.length; i++) {}
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

uint8_t hex_digit( char c ) {
#	define CASE( c, i )													\
		case c: return i
	switch (c) {
		CASE('0', 0x0);
		CASE('1', 0x1);
		CASE('2', 0x2);
		CASE('3', 0x3);
		CASE('4', 0x4);
		CASE('5', 0x5);
		CASE('6', 0x6);
		CASE('7', 0x7);
		CASE('8', 0x8);
		CASE('9', 0x9);
		CASE('a', 0xa);
		CASE('b', 0xb);
		CASE('c', 0xc);
		CASE('d', 0xd);
		CASE('e', 0xe);
		CASE('f', 0xf);
		default:
			return 0;
	}
#undef CASE
}

uint8_t parse_arg( wstr line, wstr * rest, werror * report ) {
	const char * line_start = line.text;
	uint8_t arg = 0;
	skip_wspace( &line );
	if ( line.text[0] == 's' ) {
		arg = 1 << 7;
	} else {
if ( line.text[ 0 ] != 'r' ) {
report->message = WSTR_LIT("Expected argument to start with either 's' or 'r'.");
return 0;
}
	}
	if ( ! isxdigit( line.text[ 1 ] ) ) {
		report->message = WSTR_LIT("No hex digit in argument.");
		return 0;
	}
	int shift = 0;
	rest->text = line.text + 2;
	if ( isxdigit( line.text[ 2 ] ) ) {
		shift = 4;
		arg |= hex_digit( line.text[ 2 ] );
		rest->text += 1;
	}
	rest->length = line.length - (rest->text - line_start);
	uint8_t larger = hex_digit( line.text[ 1 ] );
	if ( ((larger << shift) & ( 1 << 7 )) || isxdigit( rest->text[ 0 ] ) ) {
		report->message = WSTR_LIT("Argument is too large.");
	}
	arg |= larger << shift;
	return arg;
}

bytecode_t * wbytecode_from_filename( wstr filename ) {
	wstr file = wstr_from_filename( filename );
	wstr file_remaining = file;
	wstr line;
	size_t total_size = 0;
	int left_in_opcode = 4;
	wstr success = WSTR_LIT("success");
	werror parse_error = { (wtype *)&werror_type, success };
	warray * uncollapsed_code = warray_new( (wtype *)&opcode_type, null_wcall );
	while (file_remaining.length != 0) {
		get_next_line( file_remaining, &line, &file_remaining );
		/* printf( "Line: " ); */
		/* wstr_println( line ); */
		wstr name;
		get_opcode_name(line, &name, &line);
		/* printf( "Opcode: " ); */
		/* wstr_println( name ); */
		uint32_t opcode = get_opcode( name );
		/* printf("opcode parsed = %.8x\n", opcode); */
		/* printf( "Opcode index = %zd\n", opcode ); */
		uint8_t size = wopcode_args[opcode];
		left_in_opcode -= size + 1;
		if (left_in_opcode < 0) {
			total_size += 1;
			left_in_opcode = 4 - (size + 1);
		}
		/* printf( "Opcode size = %d\n", (int) size ); */
		for (int i = 1; i <= size; i++) {
			skip_wspace(&line);
			uint8_t arg = parse_arg(line, &line, &parse_error);
			/* printf("arg parsed = %d\n", (int)arg); */
			/* printf("arg parsed = %.2x\n", (int)arg); */
			opcode |= (uint32_t) arg << (i * 8);
			if ( i != size ) {
				skip_wspace(&line);
				fflush(stdout);
				assert( line.text[ 0 ] == ',' );
				line.text++;
				line.length--;
			}
		}
		skip_wspace( &line );
		assert( line.length == 0 || line.text[ 0 ] == '#' );
		/* printf("opcode in = %.8x\n", opcode); */
		warray_push_back( uncollapsed_code, (void *) &opcode, null_wcall );
	}
	warray_debug_print(uncollapsed_code);
	++total_size;
	/* printf("total size = %zd\n", total_size); */
	bytecode_t * code = malloc(sizeof(bytecode_t) * total_size);
	bytecode_t * code_i = code;
	bytecode_t opcode;
	uint8_t used_in_opcode = 0;
	while ( ! warray_empty( uncollapsed_code ) ) {
		warray_pop_front( uncollapsed_code, (void *) &opcode, null_wcall );
		printf("opcode = %.8x\n", opcode);
		uint8_t size = wopcode_args[opcode & 0xff] + 1;
		if (used_in_opcode + size > sizeof(bytecode_t)) {
			/* printf("next opcode\n"); */
			while (used_in_opcode < sizeof(bytecode_t)) {
				*code_i &= ~(0xff << (used_in_opcode * 8));
				++used_in_opcode;
			}
			++code_i;
			used_in_opcode = 0;
		}
		for (int i = 0; i < size; i++) {
			*code_i &= ~(0xff << ((used_in_opcode + i) * 8));
			*code_i |= ((opcode >> (i * 8)) & 0xff) << ((used_in_opcode + i) * 8);
		}
		used_in_opcode += size;
		/* printf("used_in_opcode = %d\n", used_in_opcode); */
	}
	while (used_in_opcode < sizeof(bytecode_t)) {
		*code_i &= ~(0xff << (used_in_opcode * 8));
		++used_in_opcode;
	}
	printf("Compressed code: ");
	for (int i = 0; i < total_size; i++) {
		printf("%.8x ", code[i]);
	}
	printf("\n");
	return code;
}

int main( int argc, char *argv[] ) {
	/* printf( "Starting Wick VM.\n" ); */
	/* printf( "Sizeof arena = %ld, 0x%lx\n", WICK_ARENA_ALIGNMENT, WICK_ARENA_ALIGNMENT ); */
	/* print_opcodes(); */
	/* printf( "sizeof(massively_long_test_string) %ld\n", sizeof("massively_long_test_string")); */
	/* printf( "src/test.wasm:\n%s\n", wstr_from_filename( WSTR_LIT( "src/test.wasm" ) ).text ); */
	bytecode_t * code = wbytecode_from_filename( WSTR_LIT( "src/test.wasm" ) );
	wexec_code( code );
	return 0;
}
