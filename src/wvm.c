#include <stdio.h>
#include "wvm.h"
#include "walloc.c"

#define MAX_STACK 256
#define ENV_SIZE 256

#if !defined(HAVE_COMPUTED_GOTOS) && defined(USE_COMPUTED_GOTOS)
	#error "Cannot use computed goto with this compiler."
#endif

#ifdef USE_COMPUTED_GOTOS
	#define DISPATCH goto *jump_targets[thisop & 0xff];
#else
	#define DISPATCH switch (thisop & 0xff)
#endif

void wexec_code( bytecode_t * c ) {
	bytecode_t thisop = *c;
	wasm_arg args[4];
	field_t * stack_start = walloc_simple( field_t, MAX_STACK );
	field_t * stack_end = stack_start;
	field_t * env = walloc_simple( field_t, ENV_SIZE );
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

int main( int argc, char *argv[] ) {
	printf( "Starting Wick VM.\n" );
	return 0;
}

