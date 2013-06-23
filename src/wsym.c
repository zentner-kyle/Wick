#include <wsym.h>
#include <hash.h>
#include <walloc.h>

int wsym_compare_wval ( wval a_v, wval b_v );
whash wsym_hash_wval ( wval sym );

wtable_elem_interface wsym_wtable_i = {
  &wsym_hash_wval,
  &wsym_compare_wval
  };

wsym wsym_of_wstr ( wstr s ) {
  wsym out;
  out.text = s;
  return out;
  }

int wsym_compare ( wsym a, wsym b ) {
  if ( a.id[0] != b.id[0] ) {
    return a.id[0] - b.id[0];
    }
  if ( a.id[1] != b.id[1] ) {
    return a.id[1] - b.id[1];
    }
  return wstr_compare ( a.text, b.text );
  }

int wsym_compare_wval ( wval a_v, wval b_v ) {
  wsym a = * ( wsym * ) a_v.p;
  wsym b = * ( wsym * ) b_v.p;
  return wsym_compare ( a, b );
  }

whash wsym_hash_wval ( wval sym ) {
  wsym self = * (wsym *) sym.p;
  return wsym_hash ( self );
  }

siphash_key wsym_key = { { 0x6a369e6ea9fbcd30, 0xd86d7859de5bbb27 } };

whash wsym_hash ( wsym self ) {
  uint64_t to_hash[3];
  to_hash[0] = self.id[0];
  to_hash[1] = self.id[1];
  to_hash[2] = wstr_hash ( self.text );
  return siphash_24 ( wsym_key, ( uint8_t * ) to_hash, sizeof ( uint64_t ) * 3 );
  }

void wsym_init_alloc (
    wsym * self,
    uint64_t id[2],
    const char * start,
    const char * past_end,
    enum wstr_alloc_type alloc_type
  ) {
  self->id[0] = id[0];
  self->id[1] = id[1];
  wstr_init_alloc ( &self->text, start, past_end, alloc_type );
  }

wsym * wsym_new_alloc (
    uint64_t id[2],
    const char * start,
    const char * past_end,
    enum wstr_alloc_type alloc_type 
  ) {
  wsym * to_return = walloc_simple ( wsym, 1 );
  if ( ! to_return ) {
    return NULL;
    }
  wsym_init_alloc ( to_return, id, start, past_end, alloc_type );
  return to_return;
  }
