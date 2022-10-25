// SPDX-License-Identifier: 0BSD
// Copyright (C) 2022 Ayman El Didi
#include <stdint.h>

#include "common.h"
#include "encoding/binary.h"

int
main()
{
	// Test if we can successfully read a uint32_t from a binary stream.

	const uint32_t uint32              = 0xfeffaabb;
	const uint8_t  uint32_big_endian[] = {
			 0xfe,
			 0xff,
			 0xaa,
			 0xbb,
        };
	const uint8_t uint32_little_endian[] = {
			0xbb,
			0xaa,
			0xff,
			0xfe,
	};
	uint32_t out = 0;

	assert(binary_uint32_decode(sizeof(uint32_big_endian),
			       uint32_big_endian, &out,
			       ENCODING_BYTE_ORDER_BIG) == 0);
	assert(out == uint32);

	out = 0;

	assert(binary_uint32_decode(sizeof(uint32_little_endian),
			       uint32_little_endian, &out,
			       ENCODING_BYTE_ORDER_LITTLE) == 0);
	assert(out == uint32);

	// Test if we correctly fail to read a uint32 when parameters are
	// invalid.

	out = 0;

	assert(binary_uint32_decode(4, NULL, &out, ENCODING_BYTE_ORDER_BIG) ==
			ENCODING_INVALID_NULL_POINTER);
	assert(out == 0);
	assert(binary_uint32_decode(4, uint32_big_endian, NULL,
			       ENCODING_BYTE_ORDER_BIG) ==
			ENCODING_INVALID_NULL_POINTER);
	assert(binary_uint32_decode(4, uint32_big_endian, &out, 3) ==
			ENCODING_INVALID_ARGUMENT);
	assert(binary_uint32_decode(1, uint32_big_endian, &out, 3) ==
			ENCODING_BUFFER_TOO_SMALL);
}
