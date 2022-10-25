// SPDX-License-Identifier: 0BSD
// Copyright (C) 2022 Ayman El Didi
#include <stddef.h>
#include <stdint.h>

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
	// These are the tests specified in RFC 4648.

	uint8_t out[20] = {0};

	const uint8_t empty[] = "";
	assert(base64_encode(0, empty, ARRAY_SIZEOF(out), out, base64) == 0);

	mem_set(out, 0, ARRAY_SIZEOF(out));

	// ASCII "f"
	const uint8_t valid1[] = "\x66";
	const uint8_t key1[]   = "\x5a\x67\x3d\x3d";
	assert(base64_encode(ARRAY_SIZEOF(valid1) - 1, valid1,
			       ARRAY_SIZEOF(out), out, base64) == 0);
	assert(mem_equal(key1, out, ARRAY_SIZEOF(key1) - 1));

	mem_set(out, 0, ARRAY_SIZEOF(out));

	// ASCII "fo"
	const uint8_t valid2[] = "\x66\x6f";
	const uint8_t key2[]   = "\x5a\x6d\x38\x3d";
	assert(base64_encode(ARRAY_SIZEOF(valid2) - 1, valid2,
			       ARRAY_SIZEOF(out), out, base64) == 0);
	assert(mem_equal(key2, out, ARRAY_SIZEOF(key2) - 1));

	mem_set(out, 0, ARRAY_SIZEOF(out));

	// ASCII "foo"
	const uint8_t valid3[] = "\x66\x6f\x6f";
	const uint8_t key3[]   = "\x5a\x6d\x39\x76";
	assert(base64_encode(ARRAY_SIZEOF(valid3) - 1, valid3,
			       ARRAY_SIZEOF(out), out, base64) == 0);
	assert(mem_equal(key3, out, ARRAY_SIZEOF(key3) - 1));

	mem_set(out, 0, ARRAY_SIZEOF(out));

	// ASCII "foob"
	const uint8_t valid4[] = "\x66\x6f\x6f\x62";
	const uint8_t key4[]   = "\x5a\x6d\x39\x76\x59\x67\x3d\x3d";
	assert(base64_encode(ARRAY_SIZEOF(valid4) - 1, valid4,
			       ARRAY_SIZEOF(out), out, base64) == 0);
	assert(mem_equal(key4, out, ARRAY_SIZEOF(key4) - 1));

	mem_set(out, 0, ARRAY_SIZEOF(out));

	// ASCII "fooba"
	const uint8_t valid5[] = "\x66\x6f\x6f\x62\x61";
	const uint8_t key5[]   = "\x5a\x6d\x39\x76\x59\x6d\x45\x3d";
	assert(base64_encode(ARRAY_SIZEOF(valid5) - 1, valid5,
			       ARRAY_SIZEOF(out), out, base64) == 0);
	assert(mem_equal(key5, out, ARRAY_SIZEOF(key5) - 1));

	mem_set(out, 0, ARRAY_SIZEOF(out));

	// ASCII "foobar"
	const uint8_t valid6[] = "\x66\x6f\x6f\x62\x61\x72";
	const uint8_t key6[]   = "\x5a\x6d\x39\x76\x59\x6d\x46\x79";
	assert(base64_encode(ARRAY_SIZEOF(valid6) - 1, valid6,
			       ARRAY_SIZEOF(out), out, base64) == 0);
	assert(mem_equal(key6, out, ARRAY_SIZEOF(key6) - 1));

	mem_set(out, 0, ARRAY_SIZEOF(out));

	// Special characters
	// PDw/Pz8+Pg==
	const uint8_t special_base64[] = {0x50, 0x44, 0x77, 0x2f, 0x50, 0x7a,
			0x38, 0x2b, 0x50, 0x67, 0x3d, 0x3d};
	// PDw_Pz8-Pg==
	const uint8_t special_base64url[] = {0x50, 0x44, 0x77, 0x5f, 0x50,
			0x7a, 0x38, 0x2d, 0x50, 0x67, 0x3d, 0x3d};
	// <<???>>
	const uint8_t special_result[] = {
			0x3c, 0x3c, 0x3f, 0x3f, 0x3f, 0x3e, 0x3e};

	assert(base64_encode(ARRAY_SIZEOF(special_result), special_result,
			       ARRAY_SIZEOF(special_base64), out,
			       base64) == 0);
	assert(mem_equal(special_base64, out, ARRAY_SIZEOF(special_base64)));

	assert(base64_encode(ARRAY_SIZEOF(special_result), special_result,
			       ARRAY_SIZEOF(special_base64), out,
			       sample_alphabet) == 0);
	assert(mem_equal(special_base64, out, ARRAY_SIZEOF(special_base64)));

	assert(base64_encode(ARRAY_SIZEOF(special_result), special_result,
			       ARRAY_SIZEOF(special_base64url), out,
			       base64url) == 0);
	assert(mem_equal(special_base64url, out,
			ARRAY_SIZEOF(special_base64url)));

	// Edge cases

	assert(base64_encode(ARRAY_SIZEOF(valid6) - 1, valid6, 2, out,
			       base64) == ENCODING_BUFFER_TOO_SMALL);
	assert(base64_encode(ARRAY_SIZEOF(valid6) - 1, valid6, 2, NULL,
			       base64) == ENCODING_BUFFER_TOO_SMALL);
	assert(base64_encode(0, NULL, ARRAY_SIZEOF(out), out, base64) == 0);
	assert(base64_encode(1, NULL, 0, NULL, base64) ==
			ENCODING_INVALID_NULL_POINTER);
	assert(base64_encode(1, NULL, ARRAY_SIZEOF(out), out, NULL) ==
			ENCODING_INVALID_NULL_POINTER);
	assert(base64_encode(1, NULL, 1, NULL, NULL) ==
			ENCODING_INVALID_NULL_POINTER);
}
