// SPDX-License-Identifier: BSL-1.0
// Copyright (C) 2022 Ayman El Didi
#include <stdint.h>

#include "common.h"
#include "encoding/binary.h"

int
main()
{
	// Test if we can successfully read a uint64_t from a binary stream.

	const uint64_t uint64              = 0xfeffaabbccddeeff;
	const uint8_t  uint64_big_endian[] = {
			 0xfe,
			 0xff,
			 0xaa,
			 0xbb,
			 0xcc,
			 0xdd,
			 0xee,
			 0xff,
        };
	const uint8_t uint64_little_endian[] = {
			0xff,
			0xee,
			0xdd,
			0xcc,
			0xbb,
			0xaa,
			0xff,
			0xfe,
	};
	uint64_t out = 0;

	assert(binary_uint64_decode(sizeof(uint64_big_endian),
			       uint64_big_endian, &out,
			       ENCODING_BYTE_ORDER_BIG) == 0);
	assert(out == uint64);

	out = 0;

	assert(binary_uint64_decode(sizeof(uint64_little_endian),
			       uint64_little_endian, &out,
			       ENCODING_BYTE_ORDER_LITTLE) == 0);
	assert(out == uint64);

	// Test if we correctly fail to read a uint64 when parameters are
	// invalid.

	out = 0;

	assert(binary_uint64_decode(8, NULL, &out, ENCODING_BYTE_ORDER_BIG) ==
			ENCODING_INVALID_NULL_POINTER);
	assert(out == 0);
	assert(binary_uint64_decode(8, uint64_big_endian, NULL,
			       ENCODING_BYTE_ORDER_BIG) ==
			ENCODING_INVALID_NULL_POINTER);
	assert(binary_uint64_decode(8, uint64_big_endian, &out, 3) ==
			ENCODING_INVALID_ARGUMENT);
	assert(binary_uint64_decode(1, uint64_big_endian, &out, 3) ==
			ENCODING_BUFFER_TOO_SMALL);
}
