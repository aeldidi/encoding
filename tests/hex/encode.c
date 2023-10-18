// SPDX-License-Identifier: 0BSD
// Copyright (C) 2022 Ayman El Didi
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>

#include "../../hex/hex.h"
#include "common.h"

int
main()
{
	uint8_t bytes[64] = {
			0x18,
			0x5f,
			0xff,
	};
	bytes[15] = 0xaa;
	bytes[63] = 0xcc;

	static uint8_t out[100000] = {0};

	assert(hex_encode(16, bytes, 99, out) == 0);
	assert(out[2] == '5' && out[3] == 'f');

	// Edge cases
	assert(hex_encode(0, bytes, ARRAY_SIZEOF(out), out) == 0);
	assert(hex_encode(1, bytes, 0, NULL) == ENCODING_BUFFER_TOO_SMALL);
	assert(hex_encode(16, bytes, 7, out) == ENCODING_BUFFER_TOO_SMALL);
}
