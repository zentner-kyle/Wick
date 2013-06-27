#ifdef OPCODE_LABEL
  args[0] = 0;
  args[1] = 0;
  args[2] = 0;
  args[3] = 0;
  if ( ! op_bunch ) {
    op_bunch = *++c;
    }
  #ifdef USE_COMPUTED_GOTOS
    goto *jump_targets[op_bunch & 0xff];
  #else
    break;
    #endif
  #endif

#undef OP_NAME
#undef OP_NUM_ARGS
#undef OP_N
