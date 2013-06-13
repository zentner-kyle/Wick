
#define WICK_FFI_DEF_0_ARG_FUNC( wrapped_func )  \
  void wick_call_##wrapped_func ( void ) { \
    return ( return_type ) wrapped_func ( ); \
    } \
  struct { \
    char name[] = #wrapped_func; \
    uint32_t arg_count = 0;
    } wick_info_##wrapped_func;

#define WICK_FFI_DEC_0_ARG_FUNC( wrapped_func )  \
  void wick_call_##wrapped_func ( );


#define WICK_FFI_DEF_1_ARG_FUNC( wrapped_func, arg1_type )  \
  void wick_call_##wrapped_func ( void * arg1 ) { \
    return ( return_type ) wrapped_func ( ( arg1_type * ) arg1 ); \
  struct { \
    char name[] = #wrapped_func; \

    } wick_info_##wrapped_func;

#define WICK_FFI_DEC_1_ARG_FUNC( wrapped_func, arg1_type )  \
  void wick_call_##wrapped_func ( void * arg1 );
