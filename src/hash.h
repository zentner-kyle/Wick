#include <stdint.h>
#include <stdlib.h>
//#include "siphash.h"
//= sip_keyof(sip_secret);

struct siphash_key {
	uint64_t k[2];
};

typedef struct siphash_key siphash_key;

siphash_key main_key;

uint64_t siphash_24( siphash_key key, uint8_t * msg, size_t len );

uint32_t w_hash ( char * str, size_t len );
