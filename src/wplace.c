#include "wplace.h"

void wplace_signal_error( wplace * place, werror * error ) {
  wchannel_send_nofail( place->error, (wobj *)error );
}
