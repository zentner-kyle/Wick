#include <assert.h>
#include <wint_trie.h>
#include <wval.h>

werror * wint_trie_init ( wint_trie * t ) {
  t->type = wtype_wint_trie;
  t->root = NULL;
  return w_ok;
  }

wint_trie * wint_trie_new ( void ) {
  wint_trie * t = walloc_simple ( wint_trie, 1 );
  if ( ! t ) {
    return NULL;
    }
  else {
    werror * e = wint_trie_init ( t );
    if ( e != w_ok ) {
      free ( t );
      return NULL;
      }
    return t;
    }
  }

/**
 * get_mask - Get the leftmost differing bit between x and y.
 */
wint_trie_int get_mask ( wint_trie_int x, wint_trie_int y ) {

  /* 
   * Get the differing bits between x and y. 
   * These are the bits that are set in one and not the other.
   */

  wint_trie_int o = x ^ y;

  /* 
   * Calculate the top bit set in the difference.
   * See http://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
   */

  --o;
  o |= o >>  1;
  o |= o >>  2;
  o |= o >>  4;
  o |= o >>  8;
  o |= o >> 16;
  o |= o >> 32;
  ++o;
  o = 1 | ( o >> 1);
  return o;
  }

wint_trie_int get_prefix ( wint_trie_int x, wint_trie_int mask ) {
  return ((~mask) & (~(mask - 1))) & x;
  }

werror * insert_at_leaf (
    wint_trie_node * restrict n,
    wint_trie_node ** to_n,
    wint_trie_int i
  ) {
  return w_ok;
  }

#define mask_idx( mask, i ) ( !! ( ( mask ) & ( i ) ) )

static wint_trie_child_t * get_child ( wint_trie_node * node, wint_trie_int i ) {
  assert ( (( node->mask - 1 ) & node->mask ) == 0 &&
           !"There should be only one bit set in mask" );
  return &node->children[ mask_idx ( node->mask, i ) ];
  }

#define child( node, i ) ( * ( get_child ( ( wint_trie_node *) node, i ) ) )

werror * wint_trie_insert ( wint_trie * t, wint_trie_int i, wval val ) {
  wint_trie_node * n = t->root;
  wint_trie_node * pn = n;
  wint_trie_int mask;

  /* 
   * While we haven't reached a leaf node and the prefix has no differing bits
   * to the left of the set bit in mask. 
   */

  while ( ( ! ( n->mask & 0x1 ) ) && ( i ^ n->prefix ) < n->mask ) {
    pn = n;
    n = child ( n, i ).pointer;
    }

  /*if ( n->mask & 0x1 ) {*/

    /*[> We reached an already existing leaf (the prefix may not match completely). <]*/
    

  mask = (n->mask == 0x1) ? n->mask : n->mask & ~0x1UL;

  /* There should now be only the branching bit set in mask. */
  assert ( ( ( n->mask - 1 ) & n->mask ) == 0 &&
           !"There should be only one bit set in mask" );

  /* Check if there is a spot to take already. */

  int idx = mask_idx ( mask, i );

  if ( mask_idx ( mask, n->children[ idx ].integer) != idx ) {
    assert ( n->mask == 0x1 && !"Half-leaf should have mask 0x1.\n");

    /*
     * The key in our spot doesn't belong, so we take its place.
     */

    assert ( n->children[ 0 ].integer == n->children [ 1 ].integer &&
             !"Half-leaf should have equal children.\n");
    wint_trie_int new_mask = get_mask ( n->children [ 0 ].integer, i );
    wint_trie_int new_prefix = get_prefix ( i, new_mask );

     /* Not persistent. */

    n->mask = new_mask;
    n->prefix = new_prefix;
    n->children[ idx ].integer = i;
    ( ( wint_trie_leaf * ) n )->values[ idx ] = val;
    }
  else if ( ( i ^ n->prefix ) < n->mask ) {

    /* We need to create a new node before n. */

    wint_trie_node * branch = walloc_simple ( wint_trie_node, 1 );
    wint_trie_leaf * leaf = walloc_simple ( wint_trie_leaf, 1 );
    if ( ! branch || ! leaf ) {
      free ( branch );
      free ( leaf );
      return &wick_out_of_memory;
      }
    leaf->mask = 0x1;
    leaf->children[0].integer = i;
    leaf->children[1].integer = i;
    leaf->values[0] = val;
    leaf->values[1] = val;

    branch->mask = get_mask ( i, n->prefix );
    branch->prefix = get_prefix ( i, branch->mask );
    child ( branch, i ).pointer = ( wint_trie_node * ) leaf;
    child ( branch, ~i ).pointer = n;

    /* Not presistent. */

    child ( pn, i ).pointer = branch;
    }
  else {

    /* 
     * Create a new branch node, and a new half-leaf. Re-arrange the three keys
     * and values among the two leaves.
     * Not persistent.
     */

    wint_trie_node * branch = walloc_simple ( wint_trie_node, 1 );
    wint_trie_leaf * leaf = walloc_simple ( wint_trie_leaf, 1 );
    if ( ! branch || ! leaf ) {
      free ( branch );
      free ( leaf );
      return &wick_out_of_memory;
      }
    assert ( get_mask ( n->prefix, i ) == 1 );
    branch->mask = n->mask;
    branch->prefix = n->prefix;

    /* 
     * Arbitrarily, we will put i into the new node. 
     * Either way, i will not be in the half-leaf.
     */

    branch->children[ idx ].pointer = ( wint_trie_node * ) leaf;
    branch->children[ ! idx ].pointer = n;

    leaf->mask = get_mask ( i, n->children [ idx ].integer );
    wint_trie_int new_idx = mask_idx ( leaf->mask, i );
    leaf->prefix = get_prefix ( i, leaf->mask );
    leaf->children[ ! new_idx ].integer = n->children [ idx ].integer;
    leaf->values[ ! new_idx ] = ( ( wint_trie_leaf * ) n)->values[ idx ];
    leaf->children[ new_idx ].integer = i;
    leaf->values[ new_idx ] = val;
    leaf->mask |= 0x1;

    n->mask = 0x1;
    n->prefix = n->children [ ! idx ].integer;
    n->children[ idx ].integer = n->children [ ! idx ].integer;
    /* No need to fixup n->values[ idx ]. */
    }
  return w_ok;

  /*while (n) {*/
    /*wint_trie_leaf * leaf = walloc_simple ( wint_trie_leaf, 1 );*/
    /*if ( ! leaf ) {*/
      /*return &wick_out_of_memory;*/
      /*}*/
    /*leaf->node.children[0] = i;*/
    /*leaf->node.children[1] = i;*/
    /*leaf->node.mask = 0x1;*/
    /*leaf->node.prefix = i & ~0x1L;*/
    /*leaf->values[0] = val;*/
    /*leaf->values[1] = val;*/
    /*child ( n, i ).pointer = leaf;*/
    /*}*/
  }

werror * wint_trie_get ( wint_trie * t, wint_trie_int i, wval ** val ) {
  wint_trie_node * n = t->root;
  wint_trie_int mask;
  while ( n && ! ( n->mask & 0x1 ) ) {
    n = child ( n, i ).pointer;
    }
  if ( ! n ) {
    *val = NULL;
    return w_ok;
    }
  mask = n->mask;
  if ( mask != 0x1 ) {
    mask = mask - 1;
    }

  /* There should now be only the branching bit set in mask. */

  if ( child ( n, i ).integer == i ) {
    *val = &( ( wint_trie_leaf * ) n )->values[ mask_idx ( mask, i ) ];
    return w_ok;
    }
  *val = NULL;
  return w_ok;
  }

bool wint_trie_has_key ( wint_trie * t, wint_trie_int i ) {
  wval * val;
  wint_trie_get ( t, i, &val );
  return val != NULL;
  }
