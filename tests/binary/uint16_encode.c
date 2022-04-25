// SPDX-License-Identifier: BSL-1.0
// Copyright (C) 2022 Ayman El Didi
#include <stdint.h>

#include "common.h"
#include "encoding/binary.h"

int
main()
{
	// Test if we can successfully encode a uint16_t to a binary stream.

	const uint16_t uint16              = 0xfeff;
	const uint8_t  uint16_big_endian[] = {
			 0xfe,
			 0xff,
        };
	const uint8_t uint16_little_endian[] = {
			0xff,
			0xfe,
	};
	uint8_t out[2] = {0};

	assert(binary_uint16_encode(uint16, sizeof(out), out,
			       ENCODING_BYTE_ORDER_BIG) == 0);
	assert(mem_equal(out, uint16_big_endian, sizeof(out)));

	mem_set(out, 0, sizeof(out));

	assert(binary_uint16_encode(uint16, sizeof(out), out,
			       ENCODING_BYTE_ORDER_LITTLE) == 0);
	assert(mem_equal(out, uint16_little_endian, sizeof(out)));

	// Test if we correctly fail to read a uint16 when parameters are
	// invalid.

	mem_set(out, 0, sizeof(out));

	assert(binary_uint16_encode(
			       uint16, 2, NULL, ENCODING_BYTE_ORDER_BIG) ==
			ENCODING_INVALID_NULL_POINTER);
	assert(binary_uint16_encode(uint16, 2, out, 3) ==
			ENCODING_INVALID_ARGUMENT);
	assert(binary_uint16_encode(uint16, 1, out, 3) ==
			ENCODING_BUFFER_TOO_SMALL);
}
