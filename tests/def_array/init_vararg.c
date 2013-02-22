#include <stdio.h>
#include "def_array.h"
const int last_val = ~0;
#define array_last_val last_val
#define array_t int
#include "def_array.c"

int main() {
	int_array a;
	int_array_init_with(&a, 0, 1, 2, 3, last_val);
	int_array_iter i = int_array_start(&a);
	printf( "expect {\n0\n1\n2\n3\n}\n" );
	do {
		printf( "%d\n", int_array_deref( i ) );
	} while( int_array_next( &i ) );
	return 0;
}
