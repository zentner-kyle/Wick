#include "wick_iter.h"

void wick_iter_begin(wick_iter * i, dyn sequence) {
	if (dyn_is_sequence(sequence)) {
		wick_string * str = (wick_string*)DYN(sequence);
		switch(str->id) {
			case WICK_UTF8_TAG:
				i->id = WICK_UTF8_ITER_TAG;
				break;
				/* TODO: Add support for non-UTF-8 Unicode. */
			case WICK_ARRAY_TAG:
				i->id = WICK_ARRAY_ITER_TAG;
				break;
			case WICK_ASSOC_ARRAY_TAG:
				i->id = WICK_ASSOC_ARRAY_ITER_TAG;
			case WICK_ARRAY_FIRM_TAG:
				i->id = WICK_ARRAY_FIRM_ITER_TAG;
				break;
			case WICK_ASSOC_ARRAY_FIRM_TAG:
				i->id = WICK_ASSOC_ARRAY_FIRM_ITER_TAG;
		}
	} else {
		/* TODO: Add support for user defined sequences. */
	}
}
