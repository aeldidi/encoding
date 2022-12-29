// SPDX-License-Identifier: 0BSD
// Copyright (C) 2022 Ayman El Didi
#include <inttypes.h>
#include <stddef.h>

#include "../../src/base32.h"
#include "common.h"

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
	const uint8_t rfc4648_cases[7][20] = {
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
	const uint8_t key[]   = {0x66, 0x6f, 0x6f, 0x62, 0x61, 0x72};
	uint8_t       out[20] = {0};

	for (size_t i = 0; i < ARRAY_SIZEOF(rfc4648_cases); i += 1) {
		assert(base32_decode(lens[i], rfc4648_cases[i],
				       ARRAY_SIZEOF(out), out, base32) == 0);
		(void)base32_decoded_length(lens[i], rfc4648_cases[i]);
		assert(mem_equal(key, out, i));

		mem_set(out, 0, ARRAY_SIZEOF(out));
	}

	for (size_t i = 0; i < ARRAY_SIZEOF(rfc4648_cases); i += 1) {
		assert(base32_decode(lens[i], rfc4648_cases[i],
				       ARRAY_SIZEOF(out), out,
				       sample_alphabet) == 0);
		size_t out_len = base32_decoded_length(
				lens[i], rfc4648_cases[i]);
		assert(mem_equal(key, out, out_len));

		mem_set(out, 0, ARRAY_SIZEOF(out));
	}

	uint8_t base32hex_str[] = // "EHM6I==="
			{0x45, 0x48, 0x4d, 0x36, 0x49, 0x3d, 0x3d, 0x3d};
	uint8_t base32hex_result[] = {0x74, 0x6c, 0x69};
	assert(base32_decode(ARRAY_SIZEOF(base32hex_str), base32hex_str,
			       ARRAY_SIZEOF(out), out, base32hex) == 0);
	assert(mem_equal(out, base32hex_result,
			ARRAY_SIZEOF(base32hex_result)));

	// Crockford
	uint8_t base32crockford_str[] = // "EHP6J==="
			{0x45, 0x48, 0x50, 0x36, 0x4a, 0x3d, 0x3d, 0x3d};
	assert(base32_decode(ARRAY_SIZEOF(base32crockford_str),
			       base32crockford_str, ARRAY_SIZEOF(out), out,
			       base32crockford) == 0);
	assert(mem_equal(out, base32hex_result,
			ARRAY_SIZEOF(base32hex_result)));

	uint8_t crockford2result = 0x08;
	uint8_t crockford2[]     = {
			    0x69, 0x6f, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d};
	assert(base32_decode(ARRAY_SIZEOF(crockford2), crockford2,
			       ARRAY_SIZEOF(out), out, base32crockford) == 0);
	assert(out[0] == crockford2result);

	// Incorrect padding

	assert(base32_decode(3, rfc4648_cases[0], ARRAY_SIZEOF(out), out,
			       base32) != 0);

	// Edge cases

	assert(base32_decode(0, NULL, 0, NULL, base32) == 0);
	assert(base32_decode(lens[1], rfc4648_cases[1], 0, NULL, base32) ==
			ENCODING_BUFFER_TOO_SMALL);
	assert(base32_decode(lens[1], rfc4648_cases[1], 0, NULL, base32) ==
			ENCODING_BUFFER_TOO_SMALL);
}
