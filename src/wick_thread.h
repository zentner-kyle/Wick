#ifndef WICK_THREAD_H
#define WICK_THREAD_H
#include <wick_dyn.h>

struct wick_thread {
  struct wick_thread * parent;
  };

typedef struct wick_thread wick_thread;

wick_thread * wick_thread_init ( wick_thread * p, wick_thread * t );

wick_thread * wick_thread_dyn ( wick_dyn obj );

#endif /* WICK_THREAD_H */
