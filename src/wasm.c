#include <stdio.h>
#include <string.h>
#include "wvm.h"

char * opcode_names[] =  {
	#define OPCODE_NAME
	#define OPCODE_LIST
	#include "wopcodes.c"
};

wasm_op op_of_name(char * name) {
	for ( int i = 0; strcmp( opcode_names[i], "last" ); ++i ) {
		if ( strcmp( opcode_names[i], name ) == 0 ) {
			return i;
		}
	}
	return 0xff;
}

int main( int argc, char *argv[] ) {
	printf( "Starting Wick assembler.\n" );
	return 0;
}

