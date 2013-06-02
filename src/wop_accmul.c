#define OP_NAME accmul
#define OP_NUM_ARGS 1

#include "wopcode_begin.c"

#ifdef OPCODE_BODY
acc *= arg_0;
#endif

#include "wopcode_end.c"
