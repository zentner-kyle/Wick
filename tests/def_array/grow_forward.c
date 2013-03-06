#define array_t int
#define array_default_size 8
#include "def_array.h"
#include "def_array.c"

int main() {
	int_array a;
	int k;
	int_array_init_to_size(&a, 0);
	int_array_debug_print( &a );
	assert( int_array_size( &a ) == 0 );
	for (int i = 0; i < 10; i++) {
		printf( "pushing %d\n", i);
		fflush(stdout);
		int_array_push_front( &a, i );
	}
	for (int j = 0; j < 10; j++) {
		k = int_array_pop_back( &a );
		printf( "%d == %d\n", k, j);
		fflush(stdout);
		assert( k == j );
	}
	return 0;
}
