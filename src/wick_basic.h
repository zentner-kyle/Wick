#ifndef WICK_BOOL_H
#define WICK_BOOL_H
#include <stdint.h>
#include <stdbool.h>

#define NOARGS void

#define VARARGS ...

#ifndef WICK_HAVE_BYTE
typedef uint8_t byte;
#endif

#define C_TOKEN_JOIN_INNER(_token_a_, _token_b_) _token_a_ ## _token_b_
#define C_TOKEN_JOIN(_token_a_, _token_b_) C_TOKEN_JOIN_INNER(_token_a_, _token_b_)

#define C_TOKEN2STRING(_token_) #_token_
#define C_MACRO2STRING(_token_) C_TOKEN2STRING(_token_)


#endif /* WICK_BOOL_H */
