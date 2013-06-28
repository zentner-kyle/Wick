#ifndef WHASH_H
#define WHASH_H
#include <stdint.h>
#include <wmacros.h>
#include <wtype_h.h>
#include <wobj.h>
#include <wval.h>

#include <stdint.h> /* uint64_t, uint8_t */
#include <stdlib.h> /* size_t */

typedef uint64_t hash_t;

struct siphash_key {
  uint64_t k[2];
  };

typedef struct siphash_key siphash_key;

hash_t siphash_24 ( const siphash_key key, const uint8_t * msg, const size_t len );

typedef uint64_t whash;
typedef whash ( whash_func_t ) ( wval );
typedef int ( whash_compare_func_t ) ( wval, wval );

#endif /* WHASH_H */
