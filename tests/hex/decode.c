// SPDX-License-Identifier: BSL-1.0
// Copyright (C) 2022 Ayman El Didi
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "common.h"
#include "encoding/hex.h"

int
main()
{
	// 5f
	uint8_t bytes[] = {0x35, 0x66};
	// F5
	uint8_t bytes_cap[] = {0x66, 0x35};

	uint8_t out = 0;

	assert(hex_decode(ARRAY_SIZEOF(bytes), bytes, 1, &out) == 0);
	assert(out == 0x5f);

	assert(hex_decode(ARRAY_SIZEOF(bytes_cap), bytes_cap, 1, &out) == 0);
	assert(out == 0xf5);

	// Invalid length
	assert(hex_decode(ARRAY_SIZEOF(bytes_cap) - 1, bytes_cap, 1, &out) ==
			ENCODING_INVALID_ARGUMENT);

	// Buffer too small
	assert(hex_decode(ARRAY_SIZEOF(bytes_cap) + 2, bytes_cap, 1, &out) ==
			ENCODING_BUFFER_TOO_SMALL);

	// Edge cases
	assert(hex_decode(0, bytes, 1, NULL) == ENCODING_INVALID_NULL_POINTER);
	assert(hex_decode(2, bytes, 0, NULL) == ENCODING_BUFFER_TOO_SMALL);
	assert(hex_decode(1, NULL, 1, NULL) == ENCODING_INVALID_NULL_POINTER);
}
