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
	// DeAdBeEf1234567890
	uint8_t bytes[] = {0x44, 0x65, 0x41, 0x64, 0x42, 0x65, 0x45, 0x66,
			0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
			0x30};
	assert(hex_valid(ARRAY_SIZEOF(bytes), bytes));

	// w0
	uint8_t invalid1[] = {0x77, 0x30};
	assert(!hex_valid(1, invalid1));

	// 0w
	uint8_t invalid2[] = {0x30, 0x77};
	assert(!hex_valid(1, invalid2));

	// Edge cases.
	assert(hex_valid(0, bytes));
	assert(!hex_valid(1, NULL));
}
