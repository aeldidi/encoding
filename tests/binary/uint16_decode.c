// SPDX-License-Identifier: BSL-1.0
// Copyright (C) 2022 Ayman El Didi
#include <stdint.h>

#include "common.h"
#include "encoding/binary.h"

int
main()
{
	// Test if we can successfully read a uint16_t from a binary stream.

	const uint16_t uint16              = 0xfeff;
	const uint8_t  uint16_big_endian[] = {
			 0xfe,
			 0xff,
        };
	const uint8_t uint16_little_endian[] = {
			0xff,
			0xfe,
	};
	uint16_t out = 0;

	assert(binary_uint16_decode(sizeof(uint16_big_endian),
			       uint16_big_endian, &out,
			       ENCODING_BYTE_ORDER_BIG) == 0);
	assert(out == uint16);

	out = 0;

	assert(binary_uint16_decode(sizeof(uint16_little_endian),
			       uint16_little_endian, &out,
			       ENCODING_BYTE_ORDER_LITTLE) == 0);
	assert(out == uint16);

	// Test if we correctly fail to read a uint16 when parameters are
	// invalid.

	out = 0;

	assert(binary_uint16_decode(2, NULL, &out, ENCODING_BYTE_ORDER_BIG) ==
			ENCODING_INVALID_NULL_POINTER);
	assert(out == 0);
	assert(binary_uint16_decode(2, uint16_big_endian, NULL,
			       ENCODING_BYTE_ORDER_BIG) ==
			ENCODING_INVALID_NULL_POINTER);
	assert(binary_uint16_decode(2, uint16_big_endian, &out, 3) ==
			ENCODING_INVALID_ARGUMENT);
	assert(binary_uint16_decode(1, uint16_big_endian, &out, 3) ==
			ENCODING_BUFFER_TOO_SMALL);
}
