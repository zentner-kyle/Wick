#ifndef WUTF8_H

#define WUTF8_H
#include <werror.h>
#include <stdint.h>
#include <wstr.h>
#include <stdbool.h>

extern werror invalid_utf8;

werror * wutf8_get ( uint32_t * dst, const char ** c, const char * past_end );

bool wutf8_valid ( wstr * str );

#endif /* end of include guard: WUTF8_H */
