// SPDX-License-Identifier: 0BSD
// Copyright (C) 2022 Ayman El Didi
#include <inttypes.h>
#include <stddef.h>

#include "common.h"
#include "encoding/base64.h"

static const uint8_t sample_alphabet[64] = {
		// ASCII 'A' to 'Z'
		0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a,
		0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50, 0x51, 0x52, 0x53, 0x54,
		0x55, 0x56, 0x57, 0x58, 0x59, 0x5a,
		// ASCII 'a' to 'z'
		0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a,
		0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70, 0x71, 0x72, 0x73, 0x74,
		0x75, 0x76, 0x77, 0x78, 0x79, 0x7a,
		// ASCII '0' to '9'
		0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
		0x2b, // '+'
		0x2f, // '/'
};

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

	for (size_t i = 0; i < ARRAY_SIZEOF(rfc4648_cases); i += 1) {
		assert(base64_valid(lens[i], rfc4648_cases[i], base64));
	}

	for (size_t i = 0; i < ARRAY_SIZEOF(rfc4648_cases); i += 1) {
		assert(base64_valid(lens[i], rfc4648_cases[i], base64url));
	}

	// Testing an alternative alphabet.
	for (size_t i = 0; i < ARRAY_SIZEOF(rfc4648_cases); i += 1) {
		assert(base64_valid(
				lens[i], rfc4648_cases[i], sample_alphabet));
	}

	// Check that incorrect padding returns false.
	assert(!base64_valid(3, rfc4648_cases[0], base64));

	// Check that an incorrectly placed pad character as byte 1 of a 4-byte
	// group correctly returns -1.
	const uint8_t invalid[] = {// "Zm9v=g=="
			0x5a, 0x6d, 0x39, 0x76, 0x3d, 0x67, 0x3d, 0x3d};
	assert(!base64_valid(8, invalid, base64));

	// Check that an incorrectly placed pad character as byte 2 of a 4-byte
	// group correctly returns -1.
	const uint8_t invalid2[] = {// "Zm9vY==="
			0x5a, 0x6d, 0x39, 0x76, 0x59, 0x3d, 0x3d, 0x3d};

	assert(!base64_valid(8, invalid2, base64));

	// Test that true is returned when str_len is 0.

	assert(base64_valid(0, NULL, base64));

	// Edge cases

	assert(!base64_valid(1, NULL, base64));
	assert(!base64_valid(lens[1], rfc4648_cases[1], NULL));
}
