#define OP_NAME addacc
#define OP_NUM_ARGS 2

#include "wopcode_begin.c"

#ifdef OPCODE_BODY
acc = arg_0 + arg_1;
#endif

#include "wopcode_end.c"
