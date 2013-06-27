#include <wmacros.h>

#ifndef OP_NUM_ARGS
  #define OP_NUM_ARGS 0
  #endif

#ifdef OPCODE_ARGS
  OP_NUM_ARGS
  #endif

#ifdef OPCODE_TOKEN
  OP_NAME
  #endif

#ifdef OPCODE_TARGET
  &&join_token ( target_, OP_NAME )
  #endif

#ifdef OPCODE_NAME
  string_of_macro ( OP_NAME )
  #endif

#ifdef OPCODE_LIST
  ,
  #endif

#ifdef OPCODE_LABEL
  #ifdef USE_COMPUTED_GOTOS
    join_token ( target_, OP_NAME ):
  #else
    break;
    case ( OP_N ):
    #endif
  #endif

#if OP_NUM_ARGS < 0 || OP_NUM_ARGS > OP_MAX_ARGS
  /*   }; */
  #pragma message "Opcode " OP_NAME " has illegal number of arguments " string_of_macro ( OP_NUM_ARGS ) "."
  #pragma message "Should be 0 <= " string_of_macro ( OP_NUM_ARGS ) " <= " string_of_macro ( OP_MAX_ARGS ) "."
  #error Illegal number of arguments to opcode.
  #endif

#ifdef OPCODE_BODY
  op_bunch >>= 8;
  args[0] = (op_bunch >> (0 * 8)) & 0xff;
  args[1] = (op_bunch >> (1 * 8)) & 0xff;
  args[2] = (op_bunch >> (2 * 8)) & 0xff;
  args[3] = (op_bunch >> (3 * 8)) & 0xff;
  #ifdef WICK_DEBUG
    for ( int clearing_args = OP_NUM_ARGS; clearing_args < OP_MAX_ARGS; ++clearing_args ) {
      args[clearing_args] = 0;
      }
    #endif
  op_bunch >>= 8 * OP_NUM_ARGS;
  #endif
