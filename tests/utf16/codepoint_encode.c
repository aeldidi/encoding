// SPDX-License-Identifier: 0BSD
// Copyright (C) 2022 Ayman El Didi
#include <stdint.h>

#include "common.h"
#include "encoding/binary.h"
#include "encoding/utf16.h"

int
main()
{
	uint16_t out[]    = {0, 0, 0, 0};
	uint16_t native[] = {0, 0, 0, 0};

	// Test if valid UTF-16 is correctly encoded.

	const uint32_t bom_cp                   = 0xfeff;
	const uint8_t  bom_encoded_big_endian[] = {
			 0xfe,
			 0xff, // Byte order mark
        };

	assert(utf16_codepoint_encode(bom_cp, ARRAY_SIZEOF(out), out) == 0);
	assert(binary_uint16_decode(ARRAY_SIZEOF(bom_encoded_big_endian),
			       bom_encoded_big_endian, native,
			       ENCODING_BYTE_ORDER_BIG) == 0);
	assert(native[0] == out[0]);

	// Surrogate pair
	const uint32_t grin_cp                   = 0x1f600;
	const uint8_t  grin_encoded_big_endian[] = {
			 0xd8, 0x3d, 0xde,
			 0x00, // GRINNING FACE (U+1F600)
        };

	mem_set((uint8_t*)native, 0, ARRAY_SIZEOF(native) * sizeof(*native));
	mem_set((uint8_t*)out, 0, ARRAY_SIZEOF(out) * sizeof(*out));

	assert(utf16_codepoint_encode(grin_cp, 4, out) == 0);
	assert(binary_uint16_decode(sizeof(grin_encoded_big_endian),
			       grin_encoded_big_endian, &native[0],
			       ENCODING_BYTE_ORDER_BIG) == 0);
	assert(binary_uint16_decode(2, &grin_encoded_big_endian[2], &native[1],
			       ENCODING_BYTE_ORDER_BIG) == 0);
	assert(native[0] == out[0] && native[1] == out[1]);

	mem_set((uint8_t*)native, 0, ARRAY_SIZEOF(native) * sizeof(*native));
	mem_set((uint8_t*)out, 0, ARRAY_SIZEOF(out) * sizeof(*out));

	// Test that a surrogate codepoint is correctly encoded as the Unicode
	// Replacement Character (U+FFFD)

	const uint32_t invalid1_cp           = 0xd83d;
	const uint8_t  invalid1_big_endian[] = "\xff\xfd";

	assert(utf16_codepoint_encode(invalid1_cp, ARRAY_SIZEOF(out), out) ==
			0);
	assert(binary_uint16_decode(ARRAY_SIZEOF(invalid1_big_endian) - 1,
			       invalid1_big_endian, native,
			       ENCODING_BYTE_ORDER_BIG) == 0);
	assert(native[0] == out[0]);

	mem_set((uint8_t*)native, 0, ARRAY_SIZEOF(native) * sizeof(*native));
	mem_set((uint8_t*)out, 0, ARRAY_SIZEOF(out) * sizeof(*out));

	// Test that an invalid 16-bit value is correctly decoded as the
	// Unicode Replacement Character (U+FFFD).

	const uint32_t invalid_cp = 0xffff;

	assert(utf16_codepoint_encode(invalid_cp, ARRAY_SIZEOF(out), out) ==
			0);
	assert(binary_uint16_decode(ARRAY_SIZEOF(invalid1_big_endian) - 1,
			       invalid1_big_endian, native,
			       ENCODING_BYTE_ORDER_BIG) == 0);
	assert(native[0] == out[0]);

	mem_set((uint8_t*)native, 0, ARRAY_SIZEOF(native) * sizeof(*native));
	mem_set((uint8_t*)out, 0, ARRAY_SIZEOF(out) * sizeof(*out));

	// Test that utf16_codepoint_encode correctly returns -1 times
	// the number of 16-bit values which would've been written when given
	// invalid parameters and a valid codepoint.

	assert(utf16_codepoint_encode(0, 0, NULL) ==
			ENCODING_BUFFER_TOO_SMALL);
	assert(utf16_codepoint_encode(grin_cp, 0, NULL) ==
			ENCODING_BUFFER_TOO_SMALL);

	mem_set((uint8_t*)out, 0, sizeof(out));

	// Test that the ENCODING_BUFFER_TOO_SMALL is returned when out_len is
	// too small.

	assert(utf16_codepoint_encode(grin_cp, 1, out) ==
			ENCODING_BUFFER_TOO_SMALL);
	assert(utf16_codepoint_encode(0xffff, 0, NULL) ==
			ENCODING_BUFFER_TOO_SMALL);

	// Test that ENCODING_INVALID_NULL_POINTER is returned when out is NULL.

	assert(utf16_codepoint_encode(0x11ffff, 1, NULL) ==
			ENCODING_INVALID_NULL_POINTER);

	// Testing some corner cases in the code.

	assert(utf16_codepoint_encode(0xfffe, ARRAY_SIZEOF(out), out) == 0);
	assert(utf16_codepoint_encode(UINT32_MAX, ARRAY_SIZEOF(out), out) ==
			0);
}
