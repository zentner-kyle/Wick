#ifndef ALLOC_H

#define ALLOC_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>


extern const void * end_args;

size_t round_up_power_2 ( size_t to_round );

bool is_power_2 ( size_t input );

uint32_t mask_of_pow2 ( uint32_t val );

#define alloc( type, count ) ( ( type * ) ( malloc ( sizeof ( type ) * ( count ) ) ) )


#endif /* end of include guard: ALLOC_H */
