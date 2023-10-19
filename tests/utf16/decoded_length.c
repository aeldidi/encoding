// SPDX-License-Identifier: 0BSD
// Copyright (C) 2022 Ayman El Didi
#include <inttypes.h>

#include "../../binary/binary.h"
#include "../../utf16/utf16.h"
#include "common.h"

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
	uint16_t native[] = {0, 0, 0, 0};

	for (size_t i = 0; i < ARRAY_SIZEOF(valid1_big_endian); i += 2) {
		assert(binary_uint16_decode(
				       ARRAY_SIZEOF(valid1_big_endian) - i,
				       &valid1_big_endian[i], &native[i / 2],
				       ENCODING_BYTE_ORDER_BIG) == 0);
	}

	assert(utf16_decoded_length(ARRAY_SIZEOF(native), native) == 3);

	mem_set((uint8_t*)native, 0, ARRAY_SIZEOF(native) * sizeof(*native));

	// Test that UTF-16 with an unpaired surrogate is correctly counted as
	// 2 codepoints.

	const uint8_t invalid1_big_endian[] = {0xd8, 0x3d};
	assert(binary_uint16_decode(ARRAY_SIZEOF(invalid1_big_endian),
			       invalid1_big_endian, native,
			       ENCODING_BYTE_ORDER_BIG) == 0);

	assert(utf16_decoded_length(ARRAY_SIZEOF(native), native) == 4);

	// Test if an unpaired surrogate truncated by length is correctly
	// counted as 1 codepoint.

	assert(utf16_decoded_length(1, native) == 1);

	// Test that an invalid 16-bit value is correctly counted as 1
	// codepoint.

	const uint16_t invalid_val = 0xdc00;
	assert(utf16_decoded_length(1, &invalid_val) == 1);

	const uint16_t invalid_val2 = 0xffff;
	assert(utf16_decoded_length(1, &invalid_val2) == 1);

	// Test that 0 is returned when str_len is 0

	assert(utf16_decoded_length(0, NULL) == 0);
}
