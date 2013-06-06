#include <assert.h>
#include "warray.h"
#include "wcall.h"
#include "wbuiltins.h"

bool error = false;

void report_error( void * ignored ) {
	error = true;
}

int main() {
	wbuiltins_init();
	warray a;
	wcall error_wcall = { .func = report_error, .data = NULL };
	warray_init( &a, wtype_upcast(&wtype_int), error_wcall );
	return error;
}
