// SPDX-License-Identifier: 0BSD
// Copyright (C) 2022 Ayman El Didi
#include <inttypes.h>

#include "../../binary/binary.c"
#include "common.h"

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

	// Ensure the ENCODING_BUFFER_TOO_SMALL is returned when the output
	// buffer is too strong.

	assert(binary_uint16_encode(
			       uint16, 1, out, ENCODING_BYTE_ORDER_LITTLE) ==
			ENCODING_BUFFER_TOO_SMALL);
}
