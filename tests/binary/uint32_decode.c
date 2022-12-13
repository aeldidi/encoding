// SPDX-License-Identifier: 0BSD
// Copyright (C) 2022 Ayman El Didi
#include <inttypes.h>

#include "common.h"
#include "encoding/binary.c"

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

	// Ensure the ENCODING_BUFFER_TOO_SMALL is returned when the output
	// buffer is too strong.

	assert(binary_uint32_decode(1, uint32_little_endian, &out,
			       ENCODING_BYTE_ORDER_LITTLE) ==
			ENCODING_BUFFER_TOO_SMALL);
}
