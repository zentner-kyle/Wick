#include "wmacros.h"
#include "wtype.h"
#include "wchannel.h"
#include "werror.h"

def_struct( wplace ) {
  wtype * type;
  wchannel * error;
};

void wplace_signal_error( wplace * place, werror * error );
