#include "wsym.h"

wsym wsym_of_wstr( wstr s ) {
    wsym out;
    out.text = s;
    return out;
}
