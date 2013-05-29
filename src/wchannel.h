#include "wmacros.h"
#include "wtype.h"

def_struct( wchannel ) {
	wtype * type;
	warray pending_msgs;
};

void wchannel_send_nofail( wchannel * self, wobj * msg );

struct wplace;

void wchannel_send( struct wplace * place, wchannel * self, wobj * msg );
