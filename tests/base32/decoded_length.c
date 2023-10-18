// SPDX-License-Identifier: 0BSD
// Copyright (C) 2022 Ayman El Didi
#include <inttypes.h>
#include <stddef.h>

#include "../../base32/base32.h"
#include "common.h"

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

	assert(base32_decoded_length(lens[0], rfc4648_cases[0]) == 0);
	assert(base32_decoded_length(lens[1], rfc4648_cases[1]) == 1);
	assert(base32_decoded_length(lens[2], rfc4648_cases[2]) == 2);
	assert(base32_decoded_length(lens[3], rfc4648_cases[3]) == 3);
	assert(base32_decoded_length(lens[4], rfc4648_cases[4]) == 4);
	assert(base32_decoded_length(lens[5], rfc4648_cases[5]) == 5);
	assert(base32_decoded_length(lens[6], rfc4648_cases[6]) == 6);

	// Check that passing an empty string returns 0.

	assert(base32_decoded_length(0, NULL) == 0);

	// Test edge cases in the code.

	uint8_t all_padding[] = {
			0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d};
	assert(base32_decoded_length(ARRAY_SIZEOF(all_padding), all_padding) ==
			0);
}
