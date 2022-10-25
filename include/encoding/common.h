// SPDX-License-Identifier: 0BSD
// Copyright (C) 2022 Ayman El Didi
#ifndef ENCODING_COMMON_H
#define ENCODING_COMMON_H

// encoding/common.h provides type definitions common to all modules in
// libencoding.
//
// Typically this file doesn't need to be included directly as all other
// headers in libencoding include this one.

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef ENCODING_PUBLIC
#define ENCODING_PUBLIC
#endif // ENCODING_PUBLIC

enum {
	// The Unicode Replacement Character (U+FFFD)
	ENCODING_CODEPOINT_ERROR = 0xfffd,
};

enum byte_order {
	ENCODING_BYTE_ORDER_BIG    = 0,
	ENCODING_BYTE_ORDER_LITTLE = 1,
};

enum {
	ENCODING_INVALID_NULL_POINTER = -1,
	ENCODING_INVALID_ARGUMENT     = -2,
	ENCODING_BUFFER_TOO_SMALL     = -3,
	ENCODING_NOT_POWER_OF_TWO     = -4,
};

// Returns a pointer to a string that describes the error code passed in as
// error.
ENCODING_PUBLIC
const char* encoding_strerror(const int error);

#ifdef __cplusplus
}
#endif

#endif // ENCODING_COMMON_H
