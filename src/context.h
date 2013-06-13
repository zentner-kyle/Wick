#ifndef CONTEXT_H
#define CONTEXT_H
#include "vm_h.h"

struct context {
  vm * interpreter;
};

typedef struct context context;

void alloc_error( context * ctxt );



#endif /* CONTEXT_H */
