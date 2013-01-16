/*
Copyright (c) 2012, Kyle Zentner
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

* Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef RBTREE_PREFIX
	#define RBTREE_PREFIX RBTREE_ELEM_T
#endif

#ifndef RBTREE_POSTFIX
	#define RBTREE_POSTFIX _rbtree
#endif

#define RBTREE_LONG_PREFIX C_TOKEN_JOIN(RBTREE_PREFIX, RBTREE_POSTFIX)

#ifndef RBTREE_ALLOC
	#define RBTREE_ALLOC malloc
#endif

#ifndef RBTREE_FREE
	#define RBTREE_FREE free
#endif

#define RBTREE_FUNC(func_postfix) C_TOKEN_JOIN( C_TOKEN_JOIN( RBTREE_LONG_PREFIX, _ ), func_postfix )

#define RBTREE_FUNC_P(func_postfix) C_TOKEN_JOIN( _, RBTREE_FUNC( func_postfix ) )

#ifdef RBTREE_USE_CONTEXT
	#ifndef RBTREE_CONTEXT_ARG
		#error "Need context arg to for RBTREE: e.g. , wick_context context"
	#endif
	#ifndef RBTREE_CONTEXT_T
		#error ""
	#endif
	#ifndef RBTREE_CONTEXT_NAME
		#error ""
	#endif
#endif

#ifndef RBTREE_ITERATOR_T
	#define RBTREE_ITERATOR_T C_TOKEN_JOIN(RBTREE_LONG_PREFIX, _iterator)
#endif

#ifdef RBTREE_ERROR_CHECKS
	#ifdef RBTREE_RESPOND_TO_ERRORS
		#ifndef RBTREE_CONTEXT_MEM_ERROR
			#ifndef RBTREE_CONTEXT_ERROR
				#error "RBTREE asked to respond to errors but no RBTREE_CONTEXT_ERROR provided."
			#endif
			#define RBTREE_CONTEXT_MEM_ERROR RBTREE_CONTEXT_ERROR
		#endif
		#ifdef RBTREE_CONTEXT_MEM_ERROR
			#define RBTREE_MEM_CHECK( memory_check_expr ) \
				if ( ! (memory_check_expr) ) { \
					RBTREE_CONTEXT_MEM_ERROR( RBTREE_CONEXT_NAME ) \
				}
	#else
		#define RBTREE_MEM_CHECK( memory_check_expr ) assert( memory_check_expr )
	#endif
#else
	#define RBTREE_MEM_CHECK( memory_check_expr )
#endif

/* Field accessors have the type:
 * (RBTREE_NODE_T * -> ^a)
 * In other words, they dereference their argument, 
 * but don't dereference the field.
 */

#ifdef RBTREE_INTRUSIVE_T
	#ifndef RBTREE_INTRUSIVE_FIELD
		#error "Could not find where to place data in intrusive RBTREE."
	#endif
	#define RBTREE_NODE_T RBTREE_ELEM_T
	#define RBTREE_ELEM( node ) (node)
	#define RBTREE_LEFT( node ) ((*node).(RBTREE_INTRUSIVE_FIELD).left)
	#define RBTREE_RIGHT( node ) ((*node).(RBTREE_INTRUSIVE_FIELD).right)
	#define RBTREE_COLOR( node ) ((*node).(RBTREE_INTRUSIVE_FIELD).color)
	#ifdef RBTREE_CACHE_KEYS
		#define RBTREE_KEY( node ) ((*node).(RBTREE_INTRUSIVE_FIELD).key)
	#endif
#else
	#define RBTREE_NODE_TO_ELEM( node ) ((*node).elem)
	#define RBTREE_LEFT( node ) ((*node).left)
	#define RBTREE_RIGHT( node ) ((*node).right)
	#define RBTREE_COLOR( node ) ((*node).color)
	#ifdef RBTREE_CACHE_KEYS
		#define RBTREE_KEYS( node ) (RBTREE_KEY_FUNC( (node).elem ))
	#endif
#endif

#ifdef RBTREE_INTRUSIVE
	#define RBTREE_NODE_T RBTREE_ELEM_T
	#define RBTREE_ELEM( node ) (node)
#else
	#define RBTREE_NODE_TO_ELEM( node ) ((node)->RBTREE_ELEM_NAME)
#endif

#ifndef RBTREE_NODE_T
	#define RBTREE_NODE_T RBTREE_LONG_PREFIX
  	/*  The rbtree node type needs to be defined if it hasn't been by the user,
  	 *  which would be indicated by them defining RBTREE_NODE_T or
		 *  by setting RBTREE_INTRUSIVE.
  	 */
		struct RBTREE_NODE_T {
			RBTREE_ELEM_T ELEM;
			#ifdef RBTREE_CACHE_KEYS
				RBTREE_KEY_T key;
			#endif
			uint8_t color;
			struct RB_NODE_T *left;
			struct RB_NODE_T *right;
		};

		typedef struct RBTREE_NODE_T RBTREE_NODE_T;
#endif

#ifndef RBTREE_KEY_FROM_ELEM
	#define RBTREE_KEY_FROM_ELEM( elem ) (elem)
	#define RBTREE_KEY_T RBTREE_ELEM_T
#endif

#ifndef RBTREE_VAL_FROM_ELEM
	#define RBTREE_VAL_FROM_ELEM( elem ) (elem)
	#define RBTREE_VAL_T RBTREE_ELEM_T
#endif

#ifndef RBTREE_KEY_LESS
	#define RBTREE_KEY_LESS( a, b ) ((a) <= (b))
#endif

#ifndef RBTREE_KEY_EQUAL
	#define RBTREE_KEY_EQUAL( a, b ) ((a) == (b))
#endif

#define RBTREE_RED 0
#define RBTREE_BLACK 1

/*  Iterators are more general, high-level mechanisms.
 *  They should act 'the same' under all collections.
 *  As such, there is no interface for customizing their internals.
 */

struct RBTREE_ITERATOR_T {
	RBTREE_T * top_of_tree;
	RBTREE_ELEM_T last_elem;
};

typedef struct RBTREE_ITERATOR RBTREE_ITERATOR;

RBTREE_NODE_T * RBTREE_FUNC_P( alloc )( int color,
                                        RBTREE_NODE_T * left,
                                        RBTREE_NODE_T * right,
                                        RBTREE_ELEM_T elem,
                                        RBTREE_KEY_T key
                                        RBTREE_CONTEXT_ARG ) {
	/* Allocates and nulls the pointers in just one node. */
	RBTREE_NODE_T * new_node = RBTREE_ALLOC_NODE( sizeof(RBTREE_NODE_T) );
	if ( ! new_node ) {
		RBTREE_ALLOC_ERROR( RBTREE_CONTEXT_NAME );
		return;
	}
	RBTREE_COLOR( new_node ) = color;
	RBTREE_LEFT ( new_node ) = left;
	RBTREE_RIGHT( new_node ) = right;
	RBTREE_ELEM ( new_node ) = elem;
	#ifdef RBTREE_CACHE_KEYS
		RBTREE_KEY( new_node ) = key;
	#endif
	return new_node;
}

void RBTREE_FUNC( new )( RBTREE_NODE_T ** to_store RBTREE_CONTEXT_ARG ) {
	*to_store = 0;
}

void RBTREE_FUNC( free )( RBTREE_NODE_T * to_free ) {
	if ( RBTREE_LEFT( to_free ) ) {
		RBTREE_FUNC( free_node )( RBTREE_LEFT( to_free ) );
	}
	if ( RBTREE_RIGHT( to_free ) ) {
		RBTREE_FUNC( free_node )( RBTREE_RIGHT( to_free ) );
	}
	#ifdef RBTREE_ELEM_FINALIZE
		RBTREE_ELEM_FINALIZE( RBTREE_ELEM( free_node ) );
	#endif
	RBTREE_FREE( to_free );
}

RBTREE_NODE_T * RBTREE_FUNC( copy )( RBTREE_NODE_T * source RBTREE_CONTEXT_ARG ) {
	RBTREE_NODE_T * new_node = RBTREE_ALLOC( sizeof(RBTREE_NODE_T) );
	if ( new_node ) {
		RBTREE_COLOR( new_node ) = RBTREE_COLOR( source );
		RBTREE_LEFT( new_node ) = RBTREE_FUNC( copy_node )( RBTREE_LEFT( source ) RBTREE_CTXT);
		if ( ! RBTREE_LEFT( new_node ) ) {
			RBTREE_ALLOC_ERROR( RBTREE_CONTEXT_ARG_NAME );
			return NULL;
		}
		RBTREE_RIGHT( new_node ) = RBTREE_FUNC( copy_node )( RBTREE_RIGHT( source ) RBTREE_CTXT);
		if ( ! RBTREE_RIGHT( new_node ) ) {
			RBTREE_FUNC( free_node )( RBTREE_LEFT( new_node ) );
			RBTREE_ALLOC_ERROR( RBTREE_CONTEXT_ARG_NAME );
			return NULL;
		}
		RBTREE_ELEM( new_node ) = RBTREE_ELEM( source );
	}
	return new_node;
}

/* See Anderson, A. A note on searching in a binary search tree. */
RBTREE_ELEM_T * RBTREE_FUNC( find )( RBTREE_NODE_T * tree, RBTREE_KEY_T key ) {
	RBTREE_NODE_T * candidate = NULL;
	while ( tree ) {
		if ( RBTREE_KEY_LESS( RBTREE_KEY( tree ), key ) ) {
			tree = RBTREE_LEFT( tree );
			candidate = NULL;
		} else {
			tree = RBTREE_RIGHT( tree );
			candidate = tree;
		}
	}
	if ( candidate && RBTREE_KEY_EQUAL( RBTREE_KEY( candidate ), key ) ) {
		return & RBTREE_ELEM( candidate );
	} else {
		return NULL;
	}
}

bool RBTREE_FUNC( contains )( RBTREE_NODE_T * tree, RBTREE_KEY_T key ) {
	return !! RBTREE_FUNC( find )( tree, key );
}

RBTREE_NODE_T * RBTREE_FUNC_P( balance )( int color, 
                                          RBTREE_ELEM_T elem, 
                                          RBTREE_KEY_T key, 
                                          RBTREE_NODE_T * left, 
                                          RBTREE_NODE_T * right
                                          RBTREE_CONTEXT_ARG ) {
}

RBTREE_NODE_T * RBTREE_FUNC_P( ins )( RBTREE_NODE_T * tree, 
                                      RBTREE_ELEM_T elem, 
                                      RBTREE_KEY_T key 
                                      RBTREE_CONTEXT_ARG ) {
	if ( ! tree ) {
		return RBTREE_FUNC( alloc )( RBTREE_RED, NULL, NULL, elem, key RBTREE_CTXT );
	}
	RBTREE_KEY_T tree_key = RBTREE_KEY( tree );
	if ( RBTREE_KEY_LESS( tree_key, key ) ) {
		return RBTREE_FUNC_P( balance )( RBTREE_COLOR( tree ),
                                     RBTREE_ELEM( tree ),
                                     tree_key,
                                     RBTREE_FUNC_P( ins )( RBTREE_LEFT( tree, elem, key )),
                                     RBTREE_RIGHT( tree )
                                     RBTREE_CTXT );
	} else if ( RBTREE_KEY_EQUAL( tree_key, key ) ) {
		return RBTREE_FUNC_P( balance )( RBTREE_COLOR( tree ),
                                     elem,
                                     key,
                                     RBTREE_LEFT( tree ),
                                     RBTREE_RIGHT( tree )
                                     RBTREE_CTXT );
	} else {
		return RBTREE_FUNC_P( balance )( RBTREE_COLOR( tree ),
                                     RBTREE_ELEM( tree ),
                                     tree_key,
                                     RBTREE_LEFT( tree )
                                     RBTREE_FUNC_P( ins )( RBTREE_RIGHT( tree, elem, key )),
                                     RBTREE_CTXT );
	}
}

/* See Okasaki, C. Red-Black Trees in a Functional Setting. */
RBTREE_NODE_T * RBTREE_FUNC( insert )( RBTREE_NODE_T * tree, RBTREE_ELEM_T elem RBTREE_CONTEXT_ARG ) {
	RBTREE_NODE_T * new_root = RBTREE_FUNC_P( ins )( tree, elem, RBTREE_KEY_FROM_ELEM( elem ) );
	if ( new_root ) {
		RBTREE_COLOR( new_root ) = RBTREE_BLACK;
	} else {
		RBTREE_ALLOC_ERROR( RBTREE_CONTEXT_ARG_NAME );
	}
	return new_root;
	/*RBTREE_NODE_T * new_node = RBTREE_FUNC_P( alloc )( RBTREE_RED, NULL, NULL, elem RBTREE_CTXT );*/
}

RBTREE_VAL_T RBTREE_FUNC( get )( RBTREE_NODE_T * tree, RBTREE_KEY_T key, RBTREE_VAL_T default_ ) {
	RBTREE_ELEM_T * elem = RBTREE_FUNC( find )( tree, key );
	if ( elem ) {
		return RBTREE_VAL_FROM_ELEM( *elem );
	} else {
		return default_;
	}
}
