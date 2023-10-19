// SPDX-License-Identifier: 0BSD
// Copyright (C) 2022 Ayman El Didi
#include <inttypes.h>

#include "../../binary/binary.h"
#include "common.h"

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

	// Ensure the ENCODING_BUFFER_TOO_SMALL is returned when the output
	// buffer is too strong.

	assert(binary_uint16_decode(1, uint16_little_endian, &out,
			       ENCODING_BYTE_ORDER_LITTLE) ==
			ENCODING_BUFFER_TOO_SMALL);
}
