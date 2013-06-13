#ifndef HASH_H
#define HASH_H

#include <stdint.h> /* uint64_t, uint8_t */
#include <stdlib.h> /* size_t */

typedef uint64_t hash_t;

struct siphash_key {
  uint64_t k[2];
  };

typedef struct siphash_key siphash_key;

hash_t siphash_24 ( const siphash_key key, const uint8_t * msg, const size_t len );

#endif /* included HASH_H */
