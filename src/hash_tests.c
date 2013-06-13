/*#define __USE_POSIX199309*/
#include <sys/time.h>
#include <time.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <siphash.h>
#include <hash.h>
#include <hash.c>

uint32_t tcl ( char * str ) {
  register char * string = str;
  register uint32_t result;
  register char c;
  if ( ( result = ( uint8_t )*string ) != 0 ) { 
    while ( ( c = *++string ) != 0 ) {
        result += ( result << 3 ) + ( uint8_t )c; 
      } 
    } 
  return result;
  }

uint32_t fnv ( char * str ) {
  uint32_t hash = 2166136261;
  for ( ; *str; str++ ) {
    hash = 16777619 * ( hash ^ *str );
    }
  return hash;
  }

uint32_t last_32 ( char * str, size_t len ) {
  switch ( len ) {
    case 0:
      return 0;
    case 1:
      return *str;
    case 2:
      return ( ( *str << 8 ) & * ( str+1 ) );
    case 3:
      return ( ( *str << 16 ) & ( * ( str+1 ) << 8 ) & * ( str+2 ) );
    default:
      return * ( uint32_t * ) ( str + len - 4 );
    }
  }

uint32_t sip24 ( struct sipkey key, char * msg, size_t len ) {
  struct siphash sip_state;
  sip24_init ( &sip_state, &key );
  sip24_update ( &sip_state, msg, len );
  return sip24_final ( &sip_state );
  }

#define TEST( name, body ) \
  clock_gettime ( CLOCK_REALTIME, &start ); \
  for ( int j = 0; j < rounds; j++ ) { \
    for ( int i = 0; s[i].start != end_strs.start ; i++ ) { \
      body \
      } \
    } \
  clock_gettime ( CLOCK_REALTIME, &end ); \
  printf ( name ": %Lf seconds\n", ( end.tv_sec - start.tv_sec ) + ( ( end.tv_nsec - start.tv_nsec ) / 1000000000.0L ) );

#define COLLISIONS( name, hash ) \
  memset ( table, 0, TABLE_SIZE ); \
  memset ( hash_table, 0, TABLE_SIZE * sizeof ( uint32_t ) ); \
  collisions = 0; \
  true_collisions = 0; \
  for ( int i = 0; s[i].start != end_strs.start ; i++ ) { \
    table[ TABLE_MASK & hash ] += 1; \
    if ( hash_table[ TABLE_MASK & hash ] == hash ) { \
      printf ( "True collision!\n" ); \
      true_collisions += 1; \
      } \
    hash_table[ TABLE_MASK & hash ] = hash; \
    1 ? printf ( "\tkey: %s, index: %d, hash: %.8x\n", s[i].start, TABLE_MASK & hash, hash ) : 1 ; \
    } \
  for ( int i = 0; i < TABLE_SIZE; i++ ) { \
    if ( table[i] > 1 ) { \
      collisions += table[i] - 1; \
      } \
    } \
  printf ( name ": %d collisions\n", collisions ); \
  printf ( name ": %d true collisions\n", true_collisions );

struct str {
  char * start;
  size_t len;
  };

typedef struct str str;

#define STR( s ) ( ( str ) { .start = s, .len = strlen ( s )   } )

int main ( int argc, char ** argv ) {
  struct timespec start;
  struct timespec end;
  int rounds = 100000000;
  str end_strs = STR ( "" );
  int collisions = 0;
  int true_collisions = 0;

  str s[] = {
    STR ( "" ),
    STR ( "!!" ),
    STR ( "$" ),
    STR ( " ( " ),
    STR ( " )" ),
    STR ( "++" ),
    STR ( "0" ),
    STR ( "<-" ),
    STR ( "<=" ),
    STR ( "=" ),
    STR ( "==" ),
    STR ( "=>" ),
    STR ( ">>=" ),
    STR ( "Array" ),
    STR ( "BasSpecialForm" ),
    STR ( "Control" ),
    STR ( "Data" ),
    STR ( "Environment" ),
    STR ( "Error" ),
    STR ( "Input" ),
    STR ( "Integer" ),
    STR ( "LipsVal" ),
    STR ( "LispError" ),
    STR ( "List" ),
    STR ( "Main" ),
    STR ( "Monad" ),
    STR ( "More" ),
    STR ( "NotFunction" ),
    STR ( "NumArgs" ),
    STR ( "Number" ),
    STR ( "Numeric" ),
    STR ( "Parsec" ),
    STR ( "ParserCombinators" ),
    STR ( "Result" ),
    STR ( "Some" ),
    STR ( "String" ),
    STR ( "System" ),
    STR ( "Test" ),
    STR ( "Text" ),
    STR ( "TypeMismatch" ),
    STR ( "WORDS" ),
    STR ( "[" ),
    STR ( "]" ),
    STR ( "a_longer_module_name" ),
    STR ( "a_module_name" ),
    STR ( "abort" ),
    STR ( "bar" ),
    STR ( "baz" ),
    STR ( "begin" ),
    STR ( "cons" ),
    STR ( "data" ),
    STR ( "do" ),
    STR ( "evaled" ),
    STR ( "expected" ),
    STR ( "extractValue" ),
    STR ( "float" ),
    STR ( "foo" ),
    STR ( "form" ),
    STR ( "found" ),
    STR ( "func" ),
    STR ( "genericLength" ),
    STR ( "getArgs" ),
    STR ( "hiding" ),
    STR ( "import" ),
    STR ( "in" ),
    STR ( "input" ),
    STR ( "int" ),
    STR ( "let" ),
    STR ( "liftM" ),
    STR ( "map" ),
    STR ( "message" ),
    STR ( "module" ),
    STR ( "parsed" ),
    STR ( "printf" ),
    STR ( "putStrLn" ),
    STR ( "readExpr" ),
    STR ( "return" ),
    STR ( "show" ),
    STR ( "showError" ),
    STR ( "spaces" ),
    STR ( "test" ),
    STR ( "trapError" ),
    STR ( "where" ),
    STR ( "writeln" ),
    STR ( "wrtie" ),
    STR ( "yet_another_long_module_name" ),
    STR ( "zip" ),
    STR ( "{" ),
    STR ( "  }" ),
    end_strs
    };

#define TABLE_SIZE 128
#define TABLE_MASK 0x7F
  uint8_t * table = ( uint8_t * ) malloc ( TABLE_SIZE );
  uint32_t * hash_table = ( uint32_t * ) malloc ( TABLE_SIZE * sizeof ( uint32_t ) );

  uint8_t sip_secret[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f  };
  struct sipkey *sip_key;
  sip_key = sip_keyof ( sip_secret );

  TEST ( "warmup", __asm__ volatile ( "" ); )
  TEST ( "empty", __asm__ volatile ( "" ); )
  TEST ( "tcl", tcl ( s[i].start ); )
  COLLISIONS ( "tcl", tcl ( s[i].start ) )
  TEST ( "fnv", fnv ( s[i].start ); )
  COLLISIONS ( "fnv", fnv ( s[i].start ) )
  TEST ( "last_32", last_32 ( s[i].start, s[i].len ); )
  COLLISIONS ( "last_32", last_32 ( s[i].start, s[i].len ) )
  TEST ( "sip24", sip24 ( *sip_key, s[i].start, s[i].len ); )
  COLLISIONS ( "sip24", sip24 ( *sip_key, s[i].start, s[i].len ) )
  siphash_key key = {{0xf1255325d0fd78e2, 0x9644fae0e4d88426  }};
  /*siphash_key key = {{0x00020304050607, 0x08090a0b0c0d0e0f  }};*/
  TEST ( "siphash_24", siphash_24 ( key, ( uint8_t * )s[i].start, s[i].len ); )
  COLLISIONS ( "siphash_24", ( uint32_t )siphash_24 ( key, ( uint8_t * )s[i].start, s[i].len ) )
  return 0;
  }
