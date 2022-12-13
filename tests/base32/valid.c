// SPDX-License-Identifier: 0BSD
// Copyright (C) 2022 Ayman El Didi
#include <inttypes.h>
#include <stddef.h>

#include "common.h"
#include "encoding/base32.h"

static const uint8_t sample_alphabet[32] = {
		// ASCII 'A' to 'Z'
		0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a,
		0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50, 0x51, 0x52, 0x53, 0x54,
		0x55, 0x56, 0x57, 0x58, 0x59, 0x5a,
		// ASCII '2' to '7'
		0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
		//
};

int
main()
{
	const size_t  lens[7]              = {0, 8, 8, 8, 8, 8, 16};
	const uint8_t rfc4648_cases[7][16] = {
			"",
			// "MY======"
			{0x4d, 0x59, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d},
			// "MZXQ===="
			{0x4d, 0x5a, 0x58, 0x51, 0x3d, 0x3d, 0x3d, 0x3d},
			// "MZXW6==="
			{0x4d, 0x5a, 0x58, 0x57, 0x36, 0x3d, 0x3d, 0x3d},
			// "MZXW6YQ="
			{0x4d, 0x5a, 0x58, 0x57, 0x36, 0x59, 0x51, 0x3d},
			// "MZXW6YTB"
			{0x4d, 0x5a, 0x58, 0x57, 0x36, 0x59, 0x54, 0x42},
			// "MZXW6YTBOI======"
			{0x4d, 0x5a, 0x58, 0x57, 0x36, 0x59, 0x54, 0x42, 0x4f,
					0x49, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d,
					0x3d},
	};

	for (size_t i = 0; i < ARRAY_SIZEOF(rfc4648_cases); i += 1) {
		assert(base32_valid(lens[i], rfc4648_cases[i], base32));
	}

	uint8_t base32hex_example[] = // "MTA3I2Q="
			{0x4d, 0x54, 0x41, 0x33, 0x49, 0x32, 0x51, 0x3d};
	assert(base32_valid(ARRAY_SIZEOF(base32hex_example), base32hex_example,
			base32hex));

	// Crockford
	assert(base32_valid(ARRAY_SIZEOF(base32hex_example), base32hex_example,
			base32crockford));

	// Testing an alternative alphabet.
	for (size_t i = 0; i < ARRAY_SIZEOF(rfc4648_cases); i += 1) {
		assert(base32_valid(
				lens[i], rfc4648_cases[i], sample_alphabet));
	}

	// Check that incorrect padding returns false.
	assert(!base32_valid(3, rfc4648_cases[0], base32));

	// Test that true is returned when str_len is 0.

	assert(base32_valid(0, NULL, base32));

	// Edge cases

	uint8_t all_padding[] = {
			0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d};

	assert(!base32_valid(8, all_padding, base32));
	assert(!base32_valid(1, NULL, base32));
	assert(!base32_valid(lens[1], rfc4648_cases[1], NULL));
}
