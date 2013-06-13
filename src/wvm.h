#ifndef WVM_H

#define WVM_H
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <wtype.h>
#include <wsym.h>

typedef uint32_t bytecode_t;

typedef size_t field_t;

typedef int8_t wasm_arg;

typedef uint8_t wasm_op;

wtype_base opcode_type = { ( wtype * ) &wtype_base_t, WSYM_LIT ( "opcode" ), sizeof ( uint32_t )   };

void wexec_code ( bytecode_t * c );

#define OP_MAX_ARGS 3

#endif /* end of include guard: WVM_H */
