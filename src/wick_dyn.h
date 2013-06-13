#ifndef WICK_DYN_H
#define WICK_DYN_H

struct wick_type;

struct wick_dyn {
  void * obj;
  struct wick_type * type;
};

typedef struct wick_dyn wick_dyn;

wick_dyn wick_make_dyn(void * obj, struct wick_type * type);


#endif /* WICK_DYN_H */
