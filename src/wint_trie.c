#include <assert.h>
#include <wint_trie.h>

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

  /*while ( true ) {*/

    /*[> Clear the bottom remaining bit. <]*/

    /*wint_trie_int n = ( ( o - 1 ) & o );*/

    /*if ( n == 0 ) {*/

      /*[> But not the topmost bit. <]*/

      /*break;*/
      /*}*/
    /*else {*/
      /*o = n;*/
      /*}*/
    /*}*/

  /*return o;*/
  }

werror * insert_at_leaf (
    wint_trie_node * restrict n,
    wint_trie_node ** to_n,
    wint_trie_int i
  ) {
  if ( n->mask == 1 ) {

    wint_trie_int new_mask = get_mask ( n->children[i & n->mask].integer, i );

    if ( new_mask != n->mask ) {

      /* 
       * Leaf node, storing two copies of its key.
       * Update the mask and overwrite the copy that doesn't belong.
       */

      n->mask = new_mask | 1;
      n->children[i & new_mask].integer = i;
      return w_ok;
      }
    else {

      /* No more possible differences, just set it. */

      n->children[i & 1].integer = i;
      return w_ok;
      }
    }

  /* Unset the botom bit. */

  wint_trie_int mask = n->mask - 1;
  assert ( mask != 0 && "Accidentally cleared last bit.");
  assert ( ( ( mask - 1 ) & mask ) != 0 &&
           "More than two bits set in mask." );

  wint_trie_int match = n->children[i & mask].integer;
  wint_trie_int not_match = n->children[!(i & mask)].integer;
  if ( match == i ) {

    /* Already in trie. */

    return w_ok;
    }

  wint_trie_int new_mask = get_mask ( i, match );
  if ( new_mask > mask ) {

    /* This corresponds to the new bit being farther left (big endian) */

    /* The insertion needs to logically come *before* this node. */

    wint_trie_node * n2 = walloc_simple ( wint_trie_node, 1 );
    wint_trie_node * new_leaf = walloc_simple ( wint_trie_node, 1 );
    if ( ! n2 || ! new_leaf ) {
      free ( n2 );
      free ( new_leaf );
      return &wick_out_of_memory;
      }
    n2->mask = new_mask;

    new_leaf->mask = 1;
    new_leaf->children[0].integer = i;
    new_leaf->children[1].integer = i;

    n2->children[n2->mask & i].pointer = new_leaf;
    n2->children[! ( n2->mask & i )].pointer = n;
    *to_n = n2;
    return w_ok;

    }
  else {

    /* The insertion comes *after* this node. */

    wint_trie_node * new_leaf = walloc_simple ( wint_trie_node, 1 );
    wint_trie_node * n2 = walloc_simple ( wint_trie_node, 1 );
    if ( ! n2 || ! new_leaf ) {
      free ( n2 );
      free ( new_leaf );
      return &wick_out_of_memory;
      }

    n2->mask = 1;
    n2->children[0].integer = not_match;
    n2->children[1].integer = not_match;

    /* Unset ths bit, since n is no longer a leaf. */

    n->mask = mask;

    new_leaf->mask = new_mask | 1;
    new_leaf->children[new_mask & i].integer = i;
    new_leaf->children[ ! ( new_mask & i )].integer = match;
    n->children[i & mask].pointer = new_leaf;
    n->children[! ( i & mask )].pointer = n2;
    }
  return w_ok;
  }

werror * wint_trie_insert ( wint_trie * t, wint_trie_int i ) {
  if ( ! t->root ) {
    wint_trie_node * n = walloc_simple ( wint_trie_node, 1 );
    if ( ! n ) {
      return &wick_out_of_memory;
      }
    n->mask = 1;
    n->children[0].integer = i;
    n->children[1].integer = i;
    t->root = n;
    return w_ok;
    }
  wint_trie_node * n = t->root;

  while ( ! ( n->mask & 1 ) ) {
    n = n->children[n->mask & i].pointer;
    }
  return &werror_not_implemented;
  }

bool wint_trie_get ( wint_trie * t, wint_trie_int i ) {
  if ( ! t->root ) {
    return false;
    }
  wint_trie_node * n = t->root;
  while ( ! ( n->mask & 1 ) ) {
    n = n->children[n->mask & i].pointer;
    }
  wint_trie_int mask = n->mask - 1;
  if ( mask == 0 ) {
    mask = 1;
    }
  return n->children[mask & i].integer == i;
  }
