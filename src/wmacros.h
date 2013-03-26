#ifndef WMACROS_H
#define WMACROS_H

#define declare_struct( name ) \
    struct name; \
    typedef struct name name;

#define def_struct( name ) \
	struct name; \
	typedef struct name name; \
	struct name


#endif /* end of include guard: WMACROS_H */
