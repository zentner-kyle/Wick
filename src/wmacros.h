#ifndef WMACROS_H
#define WMACROS_H


/* Structs and typedefs. */
#define declare_struct( name ) \
    struct name; \
    typedef struct name name;

#define def_struct( name ) \
	struct name; \
	typedef struct name name; \
	struct name


/* Token manipulation. */
#define join_token(a, b) join_token_inner( a, b )
#define join_token_inner( a, b ) a ## b

#define string_of_macro( m ) string_of_token( m )
#define string_of_token( t ) #t


/* Tuple manipulation. */
#define tuple2_1( t ) tuple2_1_inner( t )
#define tuple2_1_inner( a, b ) a

#define tuple2_2( t ) tuple2_2_inner( t )
#define tuple2_2_inner( a, b ) b


#endif /* end of include guard: WMACROS_H */
