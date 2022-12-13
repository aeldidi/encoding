// SPDX-License-Identifier: 0BSD
// Copyright (C) 2022 Ayman El Didi
#include <inttypes.h>
#include <stddef.h>

#include "common.h"
#include "encoding/base64.h"

int
main()
{
	const size_t  lens[7]             = {0, 4, 4, 4, 8, 8, 8};
	const uint8_t rfc4648_cases[7][8] = {
			"",
			// "Zg=="
			{0x5a, 0x67, 0x3d, 0x3d},
			// "Zm8="
			{0x5a, 0x6d, 0x38, 0x3d},
			// "Zm9v"
			{0x5a, 0x6d, 0x39, 0x76},
			// "Zm9vYg=="
			{0x5a, 0x6d, 0x39, 0x76, 0x59, 0x67, 0x3d, 0x3d},
			// "Zm9vYmE="
			{0x5a, 0x6d, 0x39, 0x76, 0x59, 0x6d, 0x45, 0x3d},
			// "Zm9vYmFy"
			{0x5a, 0x6d, 0x39, 0x76, 0x59, 0x6d, 0x46, 0x79},
	};

	assert(base64_decoded_length(lens[0], rfc4648_cases[0]) == 0);
	assert(base64_decoded_length(lens[1], rfc4648_cases[1]) == 1);
	assert(base64_decoded_length(lens[2], rfc4648_cases[2]) == 2);
	assert(base64_decoded_length(lens[3], rfc4648_cases[3]) == 3);
	assert(base64_decoded_length(lens[4], rfc4648_cases[4]) == 4);
	assert(base64_decoded_length(lens[5], rfc4648_cases[5]) == 5);
	assert(base64_decoded_length(lens[6], rfc4648_cases[6]) == 6);

	// Check that passing an empty string returns 0.

	assert(base64_decoded_length(0, NULL) == 0);

	// Test edge cases in the code.

	uint8_t all_padding[] = {0x3d, 0x3d, 0x3d, 0x3d};
	assert(base64_decoded_length(4, all_padding) == 0);
}
