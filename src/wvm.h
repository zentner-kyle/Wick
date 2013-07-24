#ifndef WVM_H
#define WVM_H

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <wtype.h>
#include <wmacros.h>

typedef uint32_t opbunch;

typedef int8_t wasm_arg;

typedef uint8_t wasm_op;

wdeclare_base ( opbunch );

void wexec_code ( opbunch * c );

#define OP_MAX_ARGS 3

#ifndef WICK_MICRO
opbunch * wbytecode_from_filename ( wstr filename );
#endif

#endif /* end of include guard: WVM_H */
