// SPDX-License-Identifier: 0BSD
// Copyright (C) 2022 Ayman El Didi
#include <inttypes.h>

#include "../../binary/binary.c"
#include "common.h"

int
main()
{
	// Test if we can successfully encode a uint32_t to a binary stream.

	const uint32_t uint32              = 0xaabbccdd;
	const uint8_t  uint32_big_endian[] = {
			 0xaa,
			 0xbb,
			 0xcc,
			 0xdd,
        };
	const uint8_t uint32_little_endian[] = {
			0xdd,
			0xcc,
			0xbb,
			0xaa,
	};
	uint8_t out[4] = {0};

	assert(binary_uint32_encode(uint32, sizeof(out), out,
			       ENCODING_BYTE_ORDER_BIG) == 0);
	assert(mem_equal(out, uint32_big_endian, 4));

	mem_set(out, 0, sizeof(out));

	assert(binary_uint32_encode(uint32, sizeof(out), out,
			       ENCODING_BYTE_ORDER_LITTLE) == 0);
	assert(mem_equal(out, uint32_little_endian, sizeof(out)));

	// Ensure the ENCODING_BUFFER_TOO_SMALL is returned when the output
	// buffer is too strong.

	assert(binary_uint32_encode(
			       uint32, 1, out, ENCODING_BYTE_ORDER_LITTLE) ==
			ENCODING_BUFFER_TOO_SMALL);
}
