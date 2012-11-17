#include "common.h"
#include "string.h"
#include <stdint.h>

#define DSER_ALLOC malloc

struct dyn {
	uint8_t * ptr;
};

typedef struct dyn dyn;

#define DYN(_dynamic_ptr_) ((_dynamic_ptr_).ptr)

struct dser_type_t {
	uint8_t bytes[33];
};

typedef struct dser_type_t dser_type_t;

struct dser_type_t {
	uint8_t bytes[33];
};

typedef struct dser_type_t dser_type_t;

struct dser_id {
	uint8_t bytes[10];
};

typedef struct dser_id dser_id;


void dser_clear_type(dser_type_t * i) {
	WICK_MEM_CHECK(i);
	memset(i, 1, 1, 0);
}

bool dser_core_type(dyn obj) {
	return (0x80 & ~*DYN(obj));
}

wick_error dser_type(dyn obj, dyn obj_out, dser_type_t * type) {
	WICK_MEM_CHECK(type);
	size_t nbytes = 0;
	if (*DYN(obj) == 0x00 ||
	    DYN(obj) == 0xFF) {
		return BAD_DATA;
	}
	dser_clear_type(type);
	if (dser_core_type(obj)) {
		type->bytes[0] = *DYN(obj);
		return WICK_SUCCESS;
	}
	switch (*DYN) {
		case 0x80:
			byte_count = 1;
			break;
		case 0x81:
			byte_count = 3;
			break;
		case 0x82:
			byte_count = 8;
			break;
		case 0x83:
			byte_count = 16;
			break;
		case 0x84:
			byte_count = 32;
			break;
	}
	for (int i = 1; i <= byte_count; ++i) {
		type->bytes[i] = *(DYN(obj) + i);
	}
	obj_out += byte_count + 1;
	return WICK_SUCCESS;
}

wick_error dser_uint(dyn obj, dyn obj_out, uint64_t * out) {
	switch (*DYN(obj)) {
		case 0x01:
			*out = *((uint8_t *) DYN(obj));
			DYN(obj_out) = DYN(obj) + 2;
			break;
		case 0x02:
			*out = *((uint16_t *) DYN(obj));
			DYN(obj_out) = DYN(obj) + 3;
			break;
		case 0x03:
			*out = *((uint32_t *) DYN(obj));
			DYN(obj_out) = DYN(obj) + 5;
			break;
		case 0x04:
			*out = *((uint64_t *) DYN(obj));
			DYN(obj_out) = DYN(obj) + 9;
			break;
		default:
			return WICK_FAILURE;
	}
}

wick_error dser_sint(dyn obj, dyn obj_out, int64_t * out) {
	switch (*DYN(obj)) {
		case 0x09:
			*out = *((uint8_t *) DYN(obj));
			DYN(obj_out) = DYN(obj) + 2;
			break;
		case 0x0A:
			*out = *((uint16_t *) DYN(obj));
			DYN(obj_out) = DYN(obj) + 3;
			break;
		case 0x0B:
			*out = *((uint32_t *) DYN(obj));
			DYN(obj_out) = DYN(obj) + 5;
			break;
		case 0x0C:
			*out = *((uint64_t *) DYN(obj));
			DYN(obj_out) = DYN(obj) + 9;
			break;
		default:
			return WICK_FAILURE;
	}
}

wick_error dser_float(dyn obj, dyn obj_out, long double * out) {
	switch (*DYN(obj)) {
		case 0x0D:
			*out = *((float *) DYN(obj));
			DYN(obj_out) = DYN(obj) + 4;
			break;
		case 0x0E:
			*out = *((double *) DYN(obj));
			DYN(obj_out) = DYN(obj) + 8;
			break;
		case 0x0F:
			*out = *((long double *) DYN(obj));
			DYN(obj_out) = DYN(obj) + 16;
			break;
		default:
			return WICK_FAILURE;
	}
}

/* wick_error dser_make_iter(dyn obj, dyn new_obj) { */
/* 	void * ptr = DSER_ALLOC() */
/* }  */
