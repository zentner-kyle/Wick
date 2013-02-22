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
	int i = 1;
	array_push_front( a, &i, error_delegate );
	++i;
	array_push_front( a, &i, error_delegate );
	++i;
	array_push_front( a, &i, error_delegate );
	i = 2;
	assert( *(int *)array_find( a, &i ) == 2 );
	printf( "%p\n", a->data );
	return error;
}
