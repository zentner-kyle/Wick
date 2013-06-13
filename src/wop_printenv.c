#define OP_NAME printenv

#include "wopcode_begin.c"

#ifdef OPCODE_BODY
  printf( "env = %p\n", env );
#endif

#include "wopcode_end.c"

