#include <stdio.h>
#include <stdint.h>
#include <time.h>
 
static uint32_t Q[4096] = {
  #include "random_seed.data"
};

static uint32_t c = 362436;

#define PHI 0x9e3779b9

int init_rand_from_dev() {
  FILE * dev_random = fopen("/dev/urandom", "r");
  if (!dev_random) {
    return -1;
  } else {
    fread(Q, sizeof(uint32_t), 4096, dev_random);
  }
  return 0;
}
 
void init_rand(uint32_t x) {
  int i;

  Q[0] = x;
  Q[1] = x + PHI;
  Q[2] = x + PHI + PHI;

  for (i = 3; i < 4096; i++)
    Q[i] = Q[i - 3] ^ Q[i - 2] ^ PHI ^ i;
}
 
/* Long period random number generator: Multiply with carry. */

/* Sources:
 *    http://en.wikipedia.org/wiki/Multiply-with-carry
 *    http://school.anhb.uwa.edu.au/personalpages/kwessen/shared/Marsaglia03.html
 */
uint32_t rand_cmwc(void) {
  uint64_t t, a = 18782LL;
  static uint32_t i = 4095;
  uint32_t x, r = 0xfffffffe;
  i = (i + 1) & 4095;
  t = a * Q[i] + c;
  c = (t >> 32);
  x = t + c;
  if (x < c) {
    x++;
    c++;
  }
  return (Q[i] = r - x);
}
