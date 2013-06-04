#ifdef OPCODE_LABEL
	*(uint32_t *) args = 0;
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
