#include "hash.h"

struct sip_hash_state {
	uint64_t v[4];
};

typedef struct sip_hash_state sip_hash_state;

#define sip_rot64( var, pos ) \
	var = ((var) << (pos) | (var) >> (64 - pos))


static inline void sip_round( sip_hash_state * s, unsigned int rounds ) {
	while ( rounds-- != 0 ) {
		s->v[0] += s->v[1];
		sip_rot64( s->v[1], 13 );
		s->v[1] ^= s->v[0];
		sip_rot64( s->v[0], 32 );

		s->v[2] += s->v[3];
		sip_rot64( s->v[3], 16 );
		s->v[3] ^= s->v[2];

		s->v[0] += s->v[3];
		sip_rot64( s->v[3], 21 );
		s->v[3] ^= s->v[0];

		s->v[2] += s->v[1];
		sip_rot64( s->v[1], 17 );
		s->v[1] ^= s->v[2];
		sip_rot64( s->v[2], 32 );
	}
}

uint64_t siphash_24( siphash_key key, uint8_t * msg, size_t len ) {
	sip_hash_state s;

	/*uint64_t v0, v1, v2, v3;*/
	uint64_t mi, m_last;
	size_t i;
	/* Number of 64 bit words */
	size_t words = len & ~7;
	s.v[0] = key.k[0] ^ 0x736f6d6570736575ull;
	s.v[1] = key.k[1] ^ 0x646f72616e646f6dull;
	s.v[2] = key.k[0] ^ 0x6c7967656e657261ull;
	s.v[3] = key.k[1] ^ 0x7465646279746573ull;

/*#define sip_round() \*/
	/*do { \*/
		/*v0 += v1; v2 += v3; \*/
		/*rot64(v1, 13); rot64(v3, 16); \*/
		/*v1 ^= v0; v3 ^= v2; \*/
		/*rot64(v0, 32); \*/
		/*v2 += v1; v0 += v3; \*/
		/*rot64(v1, 17); rot64(v3, 21); \*/
		/*v1 ^= v2; v3 ^= v0; \*/
		/*rot64(v2, 32); \*/
	/*} while (0)*/

	/* Loop for all but last seven bytes. */
	for ( i = 0; i < words; i += 8 ) {
		mi = *(uint64_t *)( msg + i );
		s.v[3] ^= mi;
		sip_round(&s, 2);
		s.v[0] ^= mi;
		/*v3 ^= mi;*/
		/*sip_round();*/
		/*sip_round();*/
		/*v0 ^= mi;*/
	}

	m_last = (uint64_t)(len & 0xff) << 56;

#define LOAD_BYTE( var, m, start, idx ) \
	case idx: var |= (uint64_t)m[ i + idx - 1 ] << ((idx - 1) * 8);

	switch( len - words ) {
		/* Load all remaining bytes into m_last. */
		LOAD_BYTE( m_last, msg, i, 7 );
		LOAD_BYTE( m_last, msg, i, 6 );
		LOAD_BYTE( m_last, msg, i, 5 );
		LOAD_BYTE( m_last, msg, i, 4 );
		LOAD_BYTE( m_last, msg, i, 3 );
		LOAD_BYTE( m_last, msg, i, 2 );
		LOAD_BYTE( m_last, msg, i, 1 );
		case 0:
		default: {}
	}

	s.v[3] ^= m_last;
	sip_round( &s, 2 );
	s.v[0] ^= m_last;

	/*v3 ^= m_last;*/
	/*sip_round();*/
	/*sip_round();*/
	/*v0 ^= m_last;*/
	
	s.v[2] ^= 0xff;
	/*v2 ^= 0xff;*/

	sip_round( &s, 4 );
	/*sip_round();*/
	/*sip_round();*/
	/*sip_round();*/
	/*sip_round();*/

	/*return v0 ^ v1 ^ v2 ^ v3;*/
	return s.v[0] ^ s.v[1] ^ s.v[2] ^ s.v[3];
}

#undef sip_rot64
/*#undef sip_round*/
#undef LOAD_BYTE


uint32_t w_hash ( char * str, size_t len ) {
	const uint32_t fnv_prime = 16777619;
	switch( len ) {
		case 0:
			return len;
		case 1:
			return *str;
		case 2:
			return (((*str << 3) ^ *(str+1)) + len) * fnv_prime;
		case 3:
			return fnv_prime * (((*str << 8) ^ (*(str+1) << 3) ^ *(str+2)) + len);
		default:
			return ((*(str+1) << 3) ^ len ^ (*str >> 3)) ^ 
				(fnv_prime * ((* (uint32_t *) (str + len - 4)) ^ ( * (uint16_t *) (str + (len / 2) )))) ;
	}
}
