// SPDX-License-Identifier: BSL-1.0
// Copyright (C) 2022 Ayman El Didi
#include <stdint.h>

#include "common.h"
#include "encoding/binary.h"

int
main()
{
	// Test if we can successfully encode a uint32_t to a binary stream.

	const uint64_t uint64              = 0xaabbccddeeff1122;
	const uint8_t  uint64_big_endian[] = {
			 0xaa,
			 0xbb,
			 0xcc,
			 0xdd,
			 0xee,
			 0xff,
			 0x11,
			 0x22,
        };
	const uint8_t uint64_little_endian[] = {
			0x22,
			0x11,
			0xff,
			0xee,
			0xdd,
			0xcc,
			0xbb,
			0xaa,
	};
	uint8_t out[8] = {0};

	assert(binary_uint64_encode(uint64, sizeof(out), out,
			       ENCODING_BYTE_ORDER_BIG) == 0);
	assert(mem_equal(out, uint64_big_endian, 8));

	mem_set(out, 0, 8);

	assert(binary_uint64_encode(uint64, sizeof(out), out,
			       ENCODING_BYTE_ORDER_LITTLE) == 0);
	assert(mem_equal(out, uint64_little_endian, sizeof(out)));

	// Test if we correctly fail to read a uint64 when parameters are
	// invalid.

	mem_set(out, 0, sizeof(out));

	assert(binary_uint64_encode(
			       uint64, 8, NULL, ENCODING_BYTE_ORDER_BIG) ==
			ENCODING_INVALID_NULL_POINTER);
	assert(binary_uint64_encode(uint64, 8, out, 3) ==
			ENCODING_INVALID_ARGUMENT);
	assert(binary_uint64_encode(uint64, 1, out, 3) ==
			ENCODING_BUFFER_TOO_SMALL);
}