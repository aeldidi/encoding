// SPDX-License-Identifier: 0BSD
// Copyright (C) 2022 Ayman El Didi
#include <inttypes.h>

#include "../../src/binary.c"
#include "../../src/utf16.h"
#include "common.h"

int
main()
{
	uint16_t out[10]    = {0};
	uint16_t native[10] = {0};

	// Test if valid UTF-16 is correctly encoded.

	const uint32_t bom_cp                   = 0xfeff;
	const uint8_t  bom_encoded_big_endian[] = {
			 0xfe, 0xff, // Byte order mark
        };

	assert(binary_uint16_decode(ARRAY_SIZEOF(bom_encoded_big_endian),
			       bom_encoded_big_endian, &native[0],
			       ENCODING_BYTE_ORDER_BIG) == 0);

	assert(utf16_encode(1, &bom_cp, ARRAY_SIZEOF(out), out) == 0);
	assert(mem_equal((uint8_t*)native, (uint8_t*)out, 2));

	mem_set((uint8_t*)out, 0, ARRAY_SIZEOF(out) * sizeof(*out));
	mem_set((uint8_t*)native, 0, ARRAY_SIZEOF(native) * sizeof(*native));

	// Surrogate pair
	const uint32_t grin_cp                   = 0x1f600;
	const uint8_t  grin_encoded_big_endian[] = {
			 0xd8, 0x3d, 0xde, 0x00, // GRINNING FACE (U+1F600)
        };

	for (size_t i = 0; i < ARRAY_SIZEOF(grin_encoded_big_endian); i += 2) {
		assert(binary_uint16_decode(
				       ARRAY_SIZEOF(grin_encoded_big_endian) -
						       i,
				       &grin_encoded_big_endian[i],
				       &native[i / 2],
				       ENCODING_BYTE_ORDER_BIG) == 0);
	}

	assert(utf16_encode(1, &grin_cp, ARRAY_SIZEOF(out), out) == 0);
	assert(mem_equal((uint8_t*)native, (uint8_t*)out, 4));

	mem_set((uint8_t*)out, 0, ARRAY_SIZEOF(out) * sizeof(*out));
	mem_set((uint8_t*)native, 0, ARRAY_SIZEOF(native) * sizeof(*native));

	// Test that a surrogate codepoint is correctly encoded as the Unicode
	// Replacement Character (U+FFFD)

	const uint32_t invalid1_cp           = 0xd83d;
	const uint8_t  invalid1_big_endian[] = "\xff\xfd";

	assert(binary_uint16_decode(ARRAY_SIZEOF(invalid1_big_endian),
			       invalid1_big_endian, &native[0],
			       ENCODING_BYTE_ORDER_BIG) == 0);

	assert(utf16_encode(1, &invalid1_cp, ARRAY_SIZEOF(out), out) == 0);
	assert(mem_equal((uint8_t*)native, (uint8_t*)out, 2));

	mem_set((uint8_t*)out, 0, ARRAY_SIZEOF(out) * sizeof(*out));

	const uint32_t invalid_cp = 0xffff;

	assert(utf16_encode(1, &invalid_cp, ARRAY_SIZEOF(out), out) == 0);
	assert(mem_equal((uint8_t*)native, (uint8_t*)out, 2));

	mem_set((uint8_t*)out, 0, ARRAY_SIZEOF(out) * sizeof(*out));
	mem_set((uint8_t*)out, 0, sizeof(out));

	assert(utf16_encode(0, &invalid_cp, 0, out) == 0);

	assert(utf16_encode(1, &invalid_cp, 0, out) ==
			ENCODING_BUFFER_TOO_SMALL);
}
