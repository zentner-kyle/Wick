#ifndef WHASH_H
#define WHASH_H
#include <stdint.h>
#include <wmacros.h>
#include <wtype_h.h>
#include <wobj.h>

typedef uint64_t whash_t;
typedef whash_t ( whash_func_t ) ( void * );
typedef int ( whash_compare_func_t ) ( void *, void * );

#endif /* WHASH_H */
