// SPDX-License-Identifier: BSL-1.0
// Copyright (C) 2022 Ayman El Didi
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "common.h"
#include "encoding/hex.h"

int
main()
{
	uint8_t        bytes[33]   = {1, 0, [32] = 0xf5};
	static uint8_t out[100000] = {0};

	// ASCII representation of results.
	static uint8_t result1[] = {0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
			0x30, 0x20, 0x20, 0x30, 0x31, 0x20, 0x30, 0x30, 0x20,
			0x30, 0x30, 0x20, 0x30, 0x30, 0x20, 0x30, 0x30, 0x20,
			0x30, 0x30, 0x20, 0x30, 0x30, 0x20, 0x30, 0x30, 0x20,
			0x20, 0x30, 0x30, 0x20, 0x30, 0x30, 0x20, 0x30, 0x30,
			0x20, 0x30, 0x30, 0x20, 0x30, 0x30, 0x20, 0x30, 0x30,
			0x20, 0x30, 0x30, 0x20, 0x30, 0x30, 0x20, 0x20, 0x7c,
			0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
			0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x7c, 0x0a,
			0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x31, 0x30, 0x20,
			0x20, 0x30, 0x30, 0x20, 0x30, 0x30, 0x20, 0x30, 0x30,
			0x20, 0x30, 0x30, 0x20, 0x30, 0x30, 0x20, 0x30, 0x30,
			0x20, 0x30, 0x30, 0x20, 0x30, 0x30, 0x20, 0x20, 0x30,
			0x30, 0x20, 0x30, 0x30, 0x20, 0x30, 0x30, 0x20, 0x30,
			0x30, 0x20, 0x30, 0x30, 0x20, 0x30, 0x30, 0x20, 0x30,
			0x30, 0x20, 0x30, 0x30, 0x20, 0x20, 0x7c, 0x2e, 0x2e,
			0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
			0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x7c, 0x0a, 0x30, 0x30,
			0x30, 0x30, 0x30, 0x30, 0x32, 0x30, 0x20, 0x20, 0x66,
			0x35, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
			0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
			0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
			0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
			0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
			0x20, 0x20, 0x20, 0x20, 0x7c, 0x2e, 0x7c, 0x0a};
	static uint8_t result2[] = {0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
			0x31, 0x20, 0x20, 0x30, 0x31, 0x20, 0x30, 0x30, 0x20,
			0x30, 0x30, 0x20, 0x30, 0x30, 0x20, 0x30, 0x30, 0x20,
			0x30, 0x30, 0x20, 0x30, 0x30, 0x20, 0x30, 0x30, 0x20,
			0x20, 0x30, 0x30, 0x20, 0x30, 0x30, 0x20, 0x30, 0x30,
			0x20, 0x30, 0x30, 0x20, 0x30, 0x30, 0x20, 0x30, 0x30,
			0x20, 0x30, 0x30, 0x20, 0x30, 0x30, 0x20, 0x20, 0x7c,
			0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
			0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x7c, 0x0a,
			0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x31, 0x31, 0x20,
			0x20, 0x30, 0x30, 0x20, 0x30, 0x30, 0x20, 0x30, 0x30,
			0x20, 0x30, 0x30, 0x20, 0x30, 0x30, 0x20, 0x30, 0x30,
			0x20, 0x30, 0x30, 0x20, 0x30, 0x30, 0x20, 0x20, 0x30,
			0x30, 0x20, 0x30, 0x30, 0x20, 0x30, 0x30, 0x20, 0x30,
			0x30, 0x20, 0x30, 0x30, 0x20, 0x30, 0x30, 0x20, 0x30,
			0x30, 0x20, 0x30, 0x30, 0x20, 0x20, 0x7c, 0x2e, 0x2e,
			0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
			0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x7c, 0x0a, 0x30, 0x30,
			0x30, 0x30, 0x30, 0x30, 0x32, 0x31, 0x20, 0x20, 0x66,
			0x35, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
			0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
			0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
			0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
			0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
			0x20, 0x20, 0x20, 0x20, 0x7c, 0x2e, 0x7c, 0x0a};
	static uint8_t result3[] = {0x64, 0x34, 0x61, 0x35, 0x31, 0x30, 0x30,
			0x30, 0x20, 0x20, 0x30, 0x31, 0x20, 0x30, 0x30, 0x20,
			0x30, 0x30, 0x20, 0x30, 0x30, 0x20, 0x30, 0x30, 0x20,
			0x30, 0x30, 0x20, 0x30, 0x30, 0x20, 0x30, 0x30, 0x20,
			0x20, 0x30, 0x30, 0x20, 0x30, 0x30, 0x20, 0x30, 0x30,
			0x20, 0x30, 0x30, 0x20, 0x30, 0x30, 0x20, 0x30, 0x30,
			0x20, 0x30, 0x30, 0x20, 0x30, 0x30, 0x20, 0x20, 0x7c,
			0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
			0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x7c, 0x0a,
			0x64, 0x34, 0x61, 0x35, 0x31, 0x30, 0x31, 0x30, 0x20,
			0x20, 0x30, 0x30, 0x20, 0x30, 0x30, 0x20, 0x30, 0x30,
			0x20, 0x30, 0x30, 0x20, 0x30, 0x30, 0x20, 0x30, 0x30,
			0x20, 0x30, 0x30, 0x20, 0x30, 0x30, 0x20, 0x20, 0x30,
			0x30, 0x20, 0x30, 0x30, 0x20, 0x30, 0x30, 0x20, 0x30,
			0x30, 0x20, 0x30, 0x30, 0x20, 0x30, 0x30, 0x20, 0x30,
			0x30, 0x20, 0x30, 0x30, 0x20, 0x20, 0x7c, 0x2e, 0x2e,
			0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
			0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x7c, 0x0a, 0x64, 0x34,
			0x61, 0x35, 0x31, 0x30, 0x32, 0x30, 0x20, 0x20, 0x66,
			0x35, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
			0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
			0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
			0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
			0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
			0x20, 0x20, 0x20, 0x20, 0x7c, 0x2e, 0x7c, 0x0a};
	static uint8_t result4[] = {0x64, 0x34, 0x61, 0x35, 0x31, 0x30, 0x30,
			0x30, 0x20, 0x20, 0x30, 0x31, 0x20, 0x30, 0x30, 0x20,
			0x30, 0x30, 0x20, 0x30, 0x30, 0x20, 0x30, 0x30, 0x20,
			0x30, 0x30, 0x20, 0x30, 0x30, 0x20, 0x30, 0x30, 0x20,
			0x20, 0x30, 0x30, 0x20, 0x30, 0x30, 0x20, 0x30, 0x30,
			0x20, 0x30, 0x30, 0x20, 0x30, 0x30, 0x20, 0x30, 0x30,
			0x20, 0x30, 0x30, 0x20, 0x30, 0x30, 0x20, 0x20, 0x7c,
			0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
			0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x7c, 0x0a,
			0x64, 0x34, 0x61, 0x35, 0x31, 0x30, 0x31, 0x30, 0x20,
			0x20, 0x30, 0x30, 0x20, 0x30, 0x30, 0x20, 0x30, 0x30,
			0x20, 0x30, 0x30, 0x20, 0x30, 0x30, 0x20, 0x30, 0x30,
			0x20, 0x30, 0x30, 0x20, 0x30, 0x30, 0x20, 0x20, 0x30,
			0x30, 0x20, 0x30, 0x30, 0x20, 0x30, 0x30, 0x20, 0x30,
			0x30, 0x20, 0x30, 0x30, 0x20, 0x30, 0x30, 0x20, 0x30,
			0x30, 0x20, 0x30, 0x30, 0x20, 0x20, 0x7c, 0x2e, 0x2e,
			0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
			0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x7c, 0x0a, 0x64, 0x34,
			0x61, 0x35, 0x31, 0x30, 0x32, 0x30, 0x20, 0x20, 0x66,
			0x35, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
			0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
			0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
			0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
			0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
			0x20, 0x20, 0x20, 0x20, 0x7c, 0x2e, 0x7c, 0x0a};

	assert(hex_dump(ARRAY_SIZEOF(bytes), bytes, ARRAY_SIZEOF(out), out,
			       NULL) == 0);
	assert(mem_equal(out, result1, ARRAY_SIZEOF(result1)));

	size_t tmp = 1;
	assert(hex_dump(ARRAY_SIZEOF(bytes), bytes, ARRAY_SIZEOF(out), out,
			       &tmp) == 0);
	assert(mem_equal(out, result2, ARRAY_SIZEOF(result2)));

	tmp = 1000000000000;
	assert(hex_dump(ARRAY_SIZEOF(bytes), bytes, ARRAY_SIZEOF(out), out,
			       &tmp) == 0);
	assert(mem_equal(out, result3, ARRAY_SIZEOF(result3)));

	assert(hex_dump(ARRAY_SIZEOF(bytes) - 1, bytes, ARRAY_SIZEOF(out), out,
			       &tmp) == 0);
	assert(mem_equal(out, result4, ARRAY_SIZEOF(result4)));

	mem_set(out, 0, ARRAY_SIZEOF(out));

	uint8_t        ascii[]       = {0x2b};
	static uint8_t asciiresult[] = {0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
			0x30, 0x30, 0x20, 0x20, 0x32, 0x62, 0x20, 0x20, 0x20,
			0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
			0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
			0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
			0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
			0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
			0x7c, 0x2b, 0x7c, 0x0a};

	assert(hex_dump(1, ascii, ARRAY_SIZEOF(out), out, NULL) == 0);
	assert(mem_equal(out, asciiresult, ARRAY_SIZEOF(asciiresult)));

	// Test if ENCODING_INVALID_ARGUMENT is returned when address would
	// overflow.
	size_t off = SIZE_MAX;
	assert(hex_dump(ARRAY_SIZEOF(bytes), bytes, ARRAY_SIZEOF(out), out,
			       &off) == ENCODING_INVALID_ARGUMENT);

	// Edge cases.
	assert(hex_dump(1, NULL, 1, out, NULL) ==
			ENCODING_INVALID_NULL_POINTER);
	assert(hex_dump(0, NULL, 1, NULL, NULL) ==
			ENCODING_INVALID_NULL_POINTER);
	assert(hex_dump(1, bytes, 0, NULL, NULL) == ENCODING_BUFFER_TOO_SMALL);
	assert(hex_dump(ARRAY_SIZEOF(bytes), bytes, 1, out, NULL) ==
			ENCODING_BUFFER_TOO_SMALL);
}