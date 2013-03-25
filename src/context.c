#include <stdio.h>
#include "context.h"

void alloc_error( context * ctxt ) {
	fprintf(stderr, "Wick: Allocation error.\n");
}
