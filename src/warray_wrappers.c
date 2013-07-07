#include <wbuiltins.h>

#define warray_elem_t int
#define warray_elem_kind integer
#include <warray.c>


#include <wvm.h>

#define warray_elem_t opbunch
#define warray_elem_kind integer
#include <warray.c>


#include <wtoken.h>

#define warray_elem_t wtoken_ptr
#define warray_elem_kind pointer
#include <warray.c>


#include <wtype_h.h>

#define warray_elem_t wtype_ptr
#define warray_elem_kind pointer
#include <warray.c>

#include <wcall.h>

#define warray_elem_t wcall_ptr
#define warray_elem_kind pointer
#include <warray.c>

#include <wobj.h>

#define warray_elem_t wobj_ptr
#define warray_elem_kind pointer
#include <warray.c>
