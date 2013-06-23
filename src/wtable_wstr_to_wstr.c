#include <wstr.h>

#define wtable_key_t wstr *
#define wtable_key_name wstr
#define wtable_key_kind pointer

#define wtable_val_t wstr *
#define wtable_val_name wstr
#define wtable_val_kind pointer

#include <wtable.c>
