#include "delegate.h"
#include <assert.h>
#include <stdlib.h>

void invoke ( delegate d ) {
	if (d.func) {
		d.func( d.data );
	}
}

const delegate null_delegate = {NULL, NULL};

