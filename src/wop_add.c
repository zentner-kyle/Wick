#define OP_NAME add
#define OP_NUM_ARGS 3

#include "wopcode_begin.c"

#ifdef OPCODE_BODY
/* printf("in add\n"); */
arg_0 = arg_1 + arg_2;
/* args[0] = args[1] + args[2]; */
#endif

#include "wopcode_end.c"
