// SPDX-License-Identifier: 0BSD
// Copyright (C) 2022 Ayman El Didi
#include <stdint.h>

#include "common.h"
#include "encoding/binary.h"
#include "encoding/utf16.h"

int
main()
{
	size_t size = 0;

	// Test if valid UTF-16 is correctly decoded.

	// This contains both a regular character as well as a surrogate pair.
	const uint8_t valid1_big_endian[] = {
			0xfe,
			0xff, // Byte order mark
			0x00,
			0x0a, // NEWLINE (U+000A)
			0xd8, 0x3d, 0xde,
			0x00, // GRINNING FACE (U+1F600)
	};
	uint16_t native[] = {0, 0, 0, 0};

	for (size_t i = 0; i < ARRAY_SIZEOF(valid1_big_endian); i += 2) {
		assert(binary_uint16_decode(
				       ARRAY_SIZEOF(valid1_big_endian) - i,
				       &valid1_big_endian[i], &native[i / 2],
				       ENCODING_BYTE_ORDER_BIG) == 0);
	}

	assert(utf16_codepoint_decode(ARRAY_SIZEOF(native), native, NULL) ==
			0xfeff);
	assert(utf16_codepoint_decode(ARRAY_SIZEOF(native), native, &size) ==
			0xfeff);
	assert(size == 1);

	size = 0;

	assert(utf16_codepoint_decode(ARRAY_SIZEOF(native) - 1, &native[1],
			       NULL) == 0x000a);
	assert(utf16_codepoint_decode(ARRAY_SIZEOF(native) - 1, &native[1],
			       &size) == 0x000a);
	assert(size == 1);

	size = 0;

	assert(utf16_codepoint_decode(ARRAY_SIZEOF(native) - 2, &native[2],
			       NULL) == 0x1f600);
	assert(utf16_codepoint_decode(ARRAY_SIZEOF(native) - 2, &native[2],
			       &size) == 0x1f600);
	assert(size == 2);

	size = 0;

	mem_set((uint8_t*)native, 0, ARRAY_SIZEOF(native) * sizeof(*native));

	// Test that UTF-16 with an unpaired surrogate is correctly decoded as
	// the Unicode Replacement Character (U+FFFD)

	const uint8_t invalid1_big_endian[] = {0xd8, 0x3d};
	assert(binary_uint16_decode(ARRAY_SIZEOF(invalid1_big_endian),
			       invalid1_big_endian, native,
			       ENCODING_BYTE_ORDER_BIG) == 0);

	assert(utf16_codepoint_decode(ARRAY_SIZEOF(native), native, NULL) ==
			ENCODING_CODEPOINT_ERROR);
	assert(utf16_codepoint_decode(ARRAY_SIZEOF(native), native, &size) ==
			ENCODING_CODEPOINT_ERROR);
	assert(size == 1);

	size = 0;

	// Test if an unpaired surrogate truncated by length is correctly
	// decoded as the Unicode Replacement Character (U+FFFD).

	assert(utf16_codepoint_decode(1, native, NULL) ==
			ENCODING_CODEPOINT_ERROR);
	assert(utf16_codepoint_decode(1, native, &size) ==
			ENCODING_CODEPOINT_ERROR);
	assert(size == 1);

	size = 0;

	// Test that an invalid 16-bit value is correctly decoded as the
	// Unicode Replacement Character (U+FFFD).

	const uint16_t invalid_val = 0xdc00;
	assert(utf16_codepoint_decode(1, &invalid_val, NULL) ==
			ENCODING_CODEPOINT_ERROR);
	assert(utf16_codepoint_decode(1, &invalid_val, &size) ==
			ENCODING_CODEPOINT_ERROR);
	assert(size == 1);

	size = 0;

	const uint16_t invalid_val2 = 0xffff;
	assert(utf16_codepoint_decode(1, &invalid_val2, NULL) ==
			ENCODING_CODEPOINT_ERROR);
	assert(utf16_codepoint_decode(1, &invalid_val2, &size) ==
			ENCODING_CODEPOINT_ERROR);
	assert(size == 1);

	size = 0;

	// Test that the values 0xffff and 0xfffe, which are valid in terms of
	// the encoding, but not valid Unicode codepoints, are correctly
	// decoded as the Unicode Replacement Character (U+FFFD).

	const uint16_t invalid_val3 = 0xffff;
	const uint16_t invalid_val4 = 0xfffe;

	assert(utf16_codepoint_decode(1, &invalid_val3, NULL) ==
			ENCODING_CODEPOINT_ERROR);
	assert(utf16_codepoint_decode(1, &invalid_val4, &size) ==
			ENCODING_CODEPOINT_ERROR);
	assert(size == 1);

	// Test that utf16_codepoint_decode correctly returns the
	// Unicode Replacement Character (U+FFFD) when passed invalid
	// parameters.

	assert(utf16_codepoint_decode(0, NULL, NULL) ==
			ENCODING_CODEPOINT_ERROR);
	assert(utf16_codepoint_decode(0, NULL, &size) ==
			ENCODING_CODEPOINT_ERROR);
	assert(size == 1);

	size = 0;

	assert(utf16_codepoint_decode(0, &invalid_val, NULL) ==
			ENCODING_CODEPOINT_ERROR);
	assert(utf16_codepoint_decode(0, &invalid_val, &size) ==
			ENCODING_CODEPOINT_ERROR);
	assert(size == 1);
}
