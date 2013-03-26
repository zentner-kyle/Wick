#include <assert.h>
#include <stdio.h>
#include "warray.h"
#include "wcall.h"
#include "wbuiltins.h"

bool error = false;

void report_error( void * ignored ) {
	error = true;
}

int main() {
	wbuiltins_init();
	wcall error_wcall = { .func = report_error, .data = NULL };
	array * a = array_new( wtype_int, error_wcall );
	printf( "%p\n", a->data );
	return error;
}
