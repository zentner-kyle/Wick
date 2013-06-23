#ifndef WHASH_H
#define WHASH_H
#include <stdint.h>
#include <wmacros.h>
#include <wtype_h.h>
#include <wobj.h>
#include <wval.h>

typedef uint64_t whash;
typedef whash ( whash_func_t ) ( wval );
typedef int ( whash_compare_func_t ) ( wval, wval );

#endif /* WHASH_H */
