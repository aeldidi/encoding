// SPDX-License-Identifier: 0BSD
// Copyright (C) 2022 Ayman El Didi
#include <stddef.h>
#include <stdint.h>

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
	size_t out = 0;

	assert(base64_decoded_length(lens[0], rfc4648_cases[0], &out) == 0);
	assert(out == 0);
	assert(base64_decoded_length(lens[1], rfc4648_cases[1], &out) == 0);
	assert(out == 1);
	assert(base64_decoded_length(lens[2], rfc4648_cases[2], &out) == 0);
	assert(out == 2);
	assert(base64_decoded_length(lens[3], rfc4648_cases[3], &out) == 0);
	assert(out == 3);
	assert(base64_decoded_length(lens[4], rfc4648_cases[4], &out) == 0);
	assert(out == 4);
	assert(base64_decoded_length(lens[5], rfc4648_cases[5], &out) == 0);
	assert(out == 5);
	assert(base64_decoded_length(lens[6], rfc4648_cases[6], &out) == 0);
	assert(out == 6);

	// Check that passing an empty string returns 0.

	assert(base64_decoded_length(0, NULL, &out) == 0);
	assert(out == 0);

	// Test edge cases in the code.

	uint8_t all_padding[] = {0x3d, 0x3d, 0x3d, 0x3d};
	assert(base64_decoded_length(4, all_padding, &out) == 0);
	assert(base64_decoded_length(1, NULL, &out) ==
			ENCODING_INVALID_NULL_POINTER);
	assert(base64_decoded_length(1, rfc4648_cases[0], NULL) ==
			ENCODING_INVALID_NULL_POINTER);
	assert(base64_decoded_length(1, NULL, NULL) ==
			ENCODING_INVALID_NULL_POINTER);
}
