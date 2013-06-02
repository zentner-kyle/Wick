#define OP_NAME add
#define OP_NUM_ARGS 3

#include "wopcode_begin.c"

#ifdef OPCODE_BODY
arg_0 = arg_1 + arg_2;
#endif

#include "wopcode_end.c"
