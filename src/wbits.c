#include "wbits.h"

const void * end_args = (void *) ~ (intptr_t)0;
/* Unaligned pointer value at end of memory space (probably in kernel space).
 * Used to indicate the last argument in a variable argument list of pointers
 * where NULL would indicate allocation failure.
 */

size_t round_up_power_2( size_t to_round ) {
  size_t next = 0;
  while ( ( next = (to_round & (to_round - 1)) ) ) {
    to_round = next;
  }
  return to_round * 2;
}

bool is_power_2( size_t input ) {
  return ! ( input & (input - 1));
}

uint32_t mask_of_pow2( uint32_t val ) {
  uint32_t result = 1;
  while ( val > 2 ) {
    result = result << 1 | result;
    val = val / 2;
  }
  return result;
}
