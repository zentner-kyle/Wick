#define OP_NAME addacc
#define OP_NUM_ARGS 2

#include <wopcode_begin.c>

#ifdef OPCODE_BODY
  acc.integer = arg_0.integer + arg_1.integer;
  #endif

#include <wopcode_end.c>
