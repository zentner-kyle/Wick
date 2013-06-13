
struct C_TOKEN_JOIN ( NAME, _matcher ) {
  matcher base;
  };

typedef struct C_TOKEN_JOIN ( NAME, _matcher ) C_TOKEN_JOIN ( NAME, _matcher );

parse_result C_TOKEN_JOIN ( NAME, _matcher_method ) ( matcher * vself, const str in ) {
  char * current = in.start;
  while ( COND ( *current ) ) {
    ++current;
    }
  return ( parse_result ) { parse_result_types.yes, {in.start, current  }, 0 };
  }


matcher * C_TOKEN_JOIN ( C_TOKEN_JOIN ( new_, NAME ), _matcher ) ( ) {
  C_TOKEN_JOIN ( NAME, _matcher ) * m = malloc ( 
      sizeof ( C_TOKEN_JOIN ( NAME, _matcher ) ) );
  if ( m ) {
    m->base.run = C_TOKEN_JOIN ( NAME, _matcher_method );
    }
  return ( matcher * ) m;
  }

#undef NAME
#undef COND
