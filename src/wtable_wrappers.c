#include <wstr.h>
#include <wbuiltins.h>


#define wtable_key_t wstr_ptr
#define wtable_key_kind pointer

#define wtable_val_t wstr_ptr
#define wtable_val_kind pointer

#include <wtable.c>

#include <wtype.h>

#define wtable_key_t wstr_ptr
#define wtable_key_kind pointer

#define wtable_val_t wtype_ptr
#define wtable_val_kind pointer
#define wtable_no_struct

#include <wtable.c>

#define wtable_key_t wstr_ptr
#define wtable_key_kind pointer

#define wtable_val_t size_t
#define wtable_val_kind uinteger
#define wtable_no_struct

#include <wtable.c>

