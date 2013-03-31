#include "wmacros.h"

#ifndef OP_NUM_ARGS
	#define OP_NUM_ARGS 0
#endif

#ifdef OPCODE_TOKEN
	OP_NAME
#endif

#ifdef OPCODE_TARGET
	&&join_token(target_, OP_NAME)
#endif

#ifdef OPCODE_NAME
	string_of_macro( OP_NAME )
#endif

#ifdef OPCODE_LIST
	,
#endif

#ifdef OPCODE_LABEL
	#ifdef USE_COMPUTED_GOTOS
		join_token(target_, OP_NAME):
	#else
		break;
		case( OP_N ):
	#endif
#endif

#if OP_NUM_ARGS < 0 || OP_NUM_ARGS > OP_MAX_ARGS
	};
	#pragma message "Opcode " OP_NAME " has illegal number of arguments " string_of_macro( OP_NUM_ARGS ) "."
	#pragma message "Should be 0 <= " string_of_macro( OP_NUM_ARGS ) " <= " string_of_macro( OP_MAX_ARGS ) "."
	#error Illegal number of arguments to opcode.
#endif

#ifdef OPCODE_BODY
	thisop >>= 8;
	#ifdef WICK_DEBUG
		*((uint32_t *)args) = thisop;
		for (int clearing_args = OP_NUM_ARGS; clearing_args < OP_MAX_ARGS; ++clearing_args) {
			args[clearing_args] = 0;
		}
	#else
		*((uint32_t *)args) = thisop;
	#endif
#endif

#undef OP_NAME
#undef OP_NUM_ARGS
#undef OP_N
