#ifndef WICK_BOOL_H
#define WICK_BOOL_H
#include <stdint.h>
#include <stdbool.h>
#include <macro_utils.h>

#define NOARGS void

#define VARARGS ...

#ifndef WICK_HAVE_BYTE
  typedef uint8_t byte;
  #endif

#endif /* WICK_BOOL_H */
