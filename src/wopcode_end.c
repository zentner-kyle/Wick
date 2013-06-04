#ifdef OPCODE_LABEL
	*(uint32_t *) args = 0;
	if ( ! op_bunch ) {
		/* printf("Loading next bunch.\n"); */
		op_bunch = *++c;
	}
	/* printf("op_bunch & 0xff = %d\n", op_bunch & 0xff); */
	#ifdef USE_COMPUTED_GOTOS
		/* printf("jump target = %p\n", jump_targets[op_bunch & 0xff]); */
		goto *jump_targets[op_bunch & 0xff];
	#else
		break;
	#endif
#endif

#undef OP_NAME
#undef OP_NUM_ARGS
#undef OP_N
