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

	// "foobar" in ASCII
	const uint8_t key[]   = {0x66, 0x6f, 0x6f, 0x62, 0x61, 0x72};
	uint8_t       out[20] = {0};

	for (size_t i = 0; i < ARRAY_SIZEOF(rfc4648_cases); i += 1) {
		assert(base64_decode(lens[i], rfc4648_cases[i],
				       ARRAY_SIZEOF(out), out, base64) == 0);
		size_t out_len = base64_decoded_length(
				lens[i], rfc4648_cases[i]);
		assert(mem_equal(key, out, out_len));

		mem_set(out, 0, ARRAY_SIZEOF(out));
	}

	for (size_t i = 0; i < ARRAY_SIZEOF(rfc4648_cases); i += 1) {
		assert(base64_decode(lens[i], rfc4648_cases[i],
				       ARRAY_SIZEOF(out), out,
				       sample_alphabet) == 0);
		size_t out_len = base64_decoded_length(
				lens[i], rfc4648_cases[i]);
		assert(mem_equal(key, out, out_len));

		mem_set(out, 0, ARRAY_SIZEOF(out));
	}

	// Incorrect padding

	assert(base64_decode(3, rfc4648_cases[0], ARRAY_SIZEOF(out), out,
			       base64) != 0);

	// PDw/Pz8+Pg==
	const uint8_t special_base64[] = {0x50, 0x44, 0x77, 0x2f, 0x50, 0x7a,
			0x38, 0x2b, 0x50, 0x67, 0x3d, 0x3d};
	// PDw_Pz8-Pg==
	const uint8_t special_base64url[] = {0x50, 0x44, 0x77, 0x5f, 0x50,
			0x7a, 0x38, 0x2d, 0x50, 0x67, 0x3d, 0x3d};
	// <<???>>
	const uint8_t special_result[] = {
			0x3c, 0x3c, 0x3f, 0x3f, 0x3f, 0x3e, 0x3e};

	assert(base64_decode(ARRAY_SIZEOF(special_base64), special_base64,
			       ARRAY_SIZEOF(out), out, base64) == 0);
	assert(mem_equal(out, special_result, ARRAY_SIZEOF(special_result)));

	assert(base64_decode(ARRAY_SIZEOF(special_base64), special_base64,
			       ARRAY_SIZEOF(out), out, sample_alphabet) == 0);
	assert(mem_equal(out, special_result, ARRAY_SIZEOF(special_result)));

	assert(base64_decode(ARRAY_SIZEOF(special_base64url),
			       special_base64url, ARRAY_SIZEOF(out), out,
			       base64url) == 0);
	assert(mem_equal(out, special_result, ARRAY_SIZEOF(special_result)));

	// Edge cases

	assert(base64_decode(0, NULL, 0, NULL, base64) == 0);
	assert(base64_decode(1, rfc4648_cases[0], 1, NULL, base64) != 0);
	assert(base64_decode(lens[1], rfc4648_cases[1], 0, NULL, base64) ==
			ENCODING_BUFFER_TOO_SMALL);
	assert(base64_decode(lens[1], rfc4648_cases[1], 0, NULL, base64) ==
			ENCODING_BUFFER_TOO_SMALL);
}
