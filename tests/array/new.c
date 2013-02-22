#include <assert.h>
#include <stdio.h>
#include "array.h"
#include "delegate.h"

bool error = false;

void report_error( void * ignored ) {
	error = true;
}

int main() {
	delegate error_delegate = { .func = report_error, .data = NULL };
	array * a = array_new( sizeof(int), error_delegate );
	printf( "%p\n", a->data );
	return error;
}
