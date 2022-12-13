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
	// These are the tests specified in RFC 4648.

	uint8_t out[20] = {0};

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
	// "foobar" in ASCII
	const uint8_t key[] = {0x66, 0x6f, 0x6f, 0x62, 0x61, 0x72};

	for (size_t i = 0; i < ARRAY_SIZEOF(rfc4648_cases); i += 1) {
		assert(base32_encode(i, key, lens[i], out, base32) == 0);
		assert(mem_equal(rfc4648_cases[i], out, lens[i]));
		mem_set(out, 0, ARRAY_SIZEOF(out));
	}

	for (size_t i = 0; i < ARRAY_SIZEOF(rfc4648_cases); i += 1) {
		assert(base32_encode(i, key, lens[i], out, sample_alphabet) ==
				0);
		assert(mem_equal(rfc4648_cases[i], out, lens[i]));
		mem_set(out, 0, ARRAY_SIZEOF(out));
	}

	uint8_t base32hex_str[]    = {0x74, 0x6c, 0x69};
	uint8_t base32hex_result[] = // "EHM6I==="
			{0x45, 0x48, 0x4d, 0x36, 0x49, 0x3d, 0x3d, 0x3d};
	assert(base32_encode(ARRAY_SIZEOF(base32hex_str), base32hex_str,
			       ARRAY_SIZEOF(out), out, base32hex) == 0);
	assert(mem_equal(out, base32hex_result,
			ARRAY_SIZEOF(base32hex_result)));

	// Crockford
	uint8_t base32crockford_result[] = // "EHP6J==="
			{0x45, 0x48, 0x50, 0x36, 0x4a, 0x3d, 0x3d, 0x3d};
	assert(base32_encode(ARRAY_SIZEOF(base32hex_str), base32hex_str,
			       ARRAY_SIZEOF(out), out, base32crockford) == 0);
	assert(mem_equal(out, base32crockford_result,
			ARRAY_SIZEOF(base32crockford_result)));

	// Edge cases

	assert(base32_encode(lens[5] - 1, rfc4648_cases[5], 2, out, base32) ==
			ENCODING_BUFFER_TOO_SMALL);
	assert(base32_encode(lens[5] - 1, rfc4648_cases[5], 2, NULL, base32) ==
			ENCODING_BUFFER_TOO_SMALL);
	assert(base32_encode(0, NULL, ARRAY_SIZEOF(out), out, base32) == 0);
}
