#define OP_NAME printstack

#include "wopcode_begin.c"

#ifdef OPCODE_BODY
	printf( "stack_start = %p\n", stack_start );
	printf( "stack_end = %p\n", stack_end );
#endif

#include "wopcode_end.c"
