#include <string.h>
#include <time.h>
#include <stdio.h>

extern int * source_ptr;
extern int * target_ptr;
void set ( int * source, int * target );

#define SIZE 10
int main ( int argc, char ** argv ) {
  int source[SIZE];
  int target[SIZE];
  set ( source, target );
  struct timespec start;
  struct timespec end;
  const unsigned long iterations = 1000L * 1000L * 1000L * 10L;
  clock_gettime ( CLOCK_REALTIME, &start );
  for ( unsigned long i = 0; i < iterations; i++) {
    memcpy ( target_ptr, source_ptr, sizeof( int ) * SIZE );
    }
  clock_gettime ( CLOCK_REALTIME, &end );

  printf ( "memcpy: %Lf seconds\n", 
           ( end.tv_sec - start.tv_sec ) + 
             ( ( end.tv_nsec - start.tv_nsec ) / 
               1000000000.0L ) );

  clock_gettime ( CLOCK_REALTIME, &start );
  for ( int i = 0; i < iterations; i++) {
    for (int i = 0; i < SIZE; i++)
    target_ptr[i] = source_ptr[i];
    }
  clock_gettime ( CLOCK_REALTIME, &end );

  printf ( "assignment: %Lf seconds\n", 
           ( end.tv_sec - start.tv_sec ) + 
             ( ( end.tv_nsec - start.tv_nsec ) / 
               1000000000.0L ) );
  }
