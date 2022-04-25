// SPDX-License-Identifier: BSL-1.0
// Copyright (C) 2022 Ayman El Didi
#include <stdint.h>

#include "common.h"
#include "encoding/binary.h"
#include "encoding/utf16.h"

int
main()
{
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
	uint16_t       native[]     = {0, 0, 0, 0};
	const uint32_t valid1_key[] = {0xfeff, 0x000a, 0x1f600};
	uint32_t       out[]        = {0, 0, 0, 0};

	for (size_t i = 0; i < ARRAY_SIZEOF(valid1_big_endian); i += 2) {
		assert(binary_uint16_decode(
				       ARRAY_SIZEOF(valid1_big_endian) - i,
				       &valid1_big_endian[i], &native[i / 2],
				       ENCODING_BYTE_ORDER_BIG) == 0);
	}

	assert(utf16_decode(ARRAY_SIZEOF(native), native, ARRAY_SIZEOF(out),
			       out) == 0);
	assert(mem_equal((uint8_t*)out, (uint8_t*)valid1_key,
			3 * sizeof(*out)));

	// Test that when the output is too small to hold the result
	// ENCODING_BUFFER_TOO_SMALL is returned.

	assert(utf16_decode(ARRAY_SIZEOF(native), native, 2, out) ==
			ENCODING_BUFFER_TOO_SMALL);

	mem_set((uint8_t*)out, 0, ARRAY_SIZEOF(out) * sizeof(*out));
	mem_set((uint8_t*)native, 0, ARRAY_SIZEOF(native) * sizeof(*native));

	// Test that UTF-16 with an unpaired surrogate is correctly decoded as
	// the Unicode Replacement Character (U+FFFD)

	const uint8_t invalid1_big_endian[] = {0xd8, 0x3d};
	assert(binary_uint16_decode(ARRAY_SIZEOF(invalid1_big_endian),
			       invalid1_big_endian, native,
			       ENCODING_BYTE_ORDER_BIG) == 0);

	assert(utf16_decode(ARRAY_SIZEOF(native), native, ARRAY_SIZEOF(out),
			       out) == 0);
	assert(out[0] == 0xfffd && out[1] == 0 && out[2] == 0 && out[3] == 0);

	mem_set((uint8_t*)out, 0, ARRAY_SIZEOF(out) * sizeof(*out));

	// Test if an unpaired surrogate truncated by length is correctly
	// decoded as the Unicode Replacement Character (U+FFFD).

	assert(utf16_decode(1, native, ARRAY_SIZEOF(out), out) == 0);
	assert(out[0] == 0xfffd && out[1] == 0 && out[2] == 0 && out[3] == 0);

	// Test that an invalid 16-bit value is correctly decoded as the
	// Unicode Replacement Character (U+FFFD).

	const uint16_t invalid_val = 0xdc00;
	assert(utf16_decode(1, &invalid_val, ARRAY_SIZEOF(out), out) == 0);
	assert(out[0] == 0xfffd && out[1] == 0 && out[2] == 0 && out[3] == 0);

	mem_set((uint8_t*)out, 0, ARRAY_SIZEOF(out) * sizeof(*out));

	const uint16_t invalid_val2 = 0xffff;
	assert(utf16_decode(1, &invalid_val2, ARRAY_SIZEOF(out), out) == 0);
	assert(out[0] == 0xfffd && out[1] == 0 && out[2] == 0 && out[3] == 0);
	assert(utf16_decode(0, &invalid_val2, ARRAY_SIZEOF(out), out) == 0);

	// Testing some edge cases in the code

	assert(utf16_decode(1, &invalid_val, 1, NULL) ==
			ENCODING_INVALID_NULL_POINTER);
	assert(utf16_decode(1, &invalid_val, 0, NULL) ==
			ENCODING_BUFFER_TOO_SMALL);
	assert(utf16_decode(1, NULL, 1, NULL) ==
			ENCODING_INVALID_NULL_POINTER);
	assert(utf16_decode(1, NULL, 1, out) == ENCODING_INVALID_NULL_POINTER);
	assert(utf16_decode(1, &invalid_val, 0, NULL) ==
			ENCODING_BUFFER_TOO_SMALL);
}
