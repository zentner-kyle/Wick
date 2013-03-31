#ifdef OPCODE_LABEL
	*(uint32_t *) args = 0;
	#ifdef USE_COMPUTED_GOTOS
		if ( ! thisop ) {
			thisop = *++c;
		}
		goto *jump_targets[thisop & 0xff];
	#else
		break;
	#endif
#endif
