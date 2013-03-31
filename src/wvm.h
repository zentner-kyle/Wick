#ifndef WVM_H

#define WVM_H
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

typedef uint32_t bytecode_t;

typedef size_t field_t;

typedef uint8_t wasm_arg;

typedef uint8_t wasm_op;

void wexec_code( bytecode_t * c );

#define OP_MAX_ARGS 3

#endif /* end of include guard: WVM_H */
