#include <string.h>
//#include <sys/time.h>
#include <time.h>
#include <stdio.h>

extern int * source_ptr;
extern int * target_ptr;
extern int size;

int main ( int argc, char ** argv ) {
  struct timespec start;
  struct timespec end;
  const unsigned long iterations = 1000 * 1000 * 1000 * 10;
  clock_gettime ( CLOCK_REALTIME, &start );
  for ( unsigned long i = 0; i < iterations; i++) {
    memcpy ( target_ptr, source_ptr, sizeof( int ) * size );
    }
  clock_gettime ( CLOCK_REALTIME, &end );

  printf ( "memcpy: %Lf seconds\n", 
           ( end.tv_sec - start.tv_sec ) + 
             ( ( end.tv_nsec - start.tv_nsec ) / 
               1000000000.0L ) );

  clock_gettime ( CLOCK_REALTIME, &start );
  for ( int i = 0; i < iterations; i++) {
    for (int i = 0; i < size; i++)
    target_ptr[i] = source_ptr[i];
    }
  clock_gettime ( CLOCK_REALTIME, &end );

  printf ( "assignment: %Lf seconds\n", 
           ( end.tv_sec - start.tv_sec ) + 
             ( ( end.tv_nsec - start.tv_nsec ) / 
               1000000000.0L ) );
  }
