#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <wbits.h>

#define macro_join_inner( _token_a_, _token_b_ ) _token_a_ ## _token_b_
#define macro_join( _token_a_, _token_b_ ) macro_join_inner ( _token_a_, _token_b_ )

#define token2string( _token_ ) #_token_
#define macro2string( _token_ ) token2string ( _token_ )
