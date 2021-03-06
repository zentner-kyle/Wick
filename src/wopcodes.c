#ifdef OPCODE_TARGET
  #define OPCODE_LIST
  #endif

#define OP_N 0
#include <wop_dummy.c>

#define OP_N 1
#include <wop_printacc.c>

#define OP_N 2
#include <wop_printenv.c>

#define OP_N 3
#include <wop_printstack.c>

#define OP_N 4
#include <wop_exit.c>

#define OP_N 5
#include <wop_addacc.c>

#define OP_N 6
#include <wop_add.c>

#define OP_N 7
#include <wop_accmul.c>

#define OP_N 8
#include <wop_accset.c>

/* Must be before last opcode. */
#undef OPCODE_LIST

#define OP_N 0xff
#include <wop_last.c>

#if defined ( OPCODE_LABEL ) && !defined ( USE_COMPUTED_GOTOS )
  default:
  assert ( 0 );
  #endif

#undef OPCODE_TARGET
#undef OPCODE_LABEL
#undef OPCODE_BODY
#undef OPCODE_NAME
#undef OPCODE_ARGS
