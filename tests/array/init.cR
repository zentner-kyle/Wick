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
	array a;
	wcall error_wcall = { .func = report_error, .data = NULL };
	array_init( &a, wtype_int, error_wcall );
	return error;
}
