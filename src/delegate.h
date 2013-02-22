#ifndef DELEGATE_H

#define DELEGATE_H

struct delegate {
	void (* func)( void * data );
	void * data;
};

typedef struct delegate delegate;

void invoke ( delegate d );

const delegate null_delegate;

#endif /* end of include guard: DELEGATE_H */
