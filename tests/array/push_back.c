#include <assert.h>
#include <stdio.h>
#include "array.h"
#include "delegate.h"

bool error = false;

void report_error( void * ignored ) {
	error = true;
}

int main() {
	array a;
	delegate error_delegate = { .func = report_error, .data = NULL };
	array_init( &a, sizeof( int ), error_delegate );
	int i = 100;
	array_push_back( &a, &i, error_delegate );
	printf( "array.data[0] == %d\n", *(int *)a.data );
	assert( *(int *)a.data == 100 );
	return error;
}
