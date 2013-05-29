
#include "stdbool.h"
#include "wmacros.h"

#define type_var join_token(wtype_type, _t)

wtype type_var;

wtype_type * join_token(wtype_type, _of_wobj)(wobj * obj) {
    if (obj && obj->type == &type_var) {
        return (wtype_type *) obj;
    } else {
        return NULL;
    }
}

#undef wtype_type
#undef type_var
