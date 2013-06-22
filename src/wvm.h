#ifndef WVM_H
#define WVM_H

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <wtype.h>
#include <wsym.h>

typedef uint32_t opbunch;

typedef size_t field_t;

typedef int8_t wasm_arg;

typedef uint8_t wasm_op;

extern wtype_base opbunch_type_base;

#define opbunch_type ((wtype *) &opbunch_type_base)

void wexec_code ( opbunch * c );

#define OP_MAX_ARGS 3

opbunch * wbytecode_from_filename ( wstr filename );

#endif /* end of include guard: WVM_H */
