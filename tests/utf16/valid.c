#include <inttypes.h>

#include "../../src/binary.c"
#include "../../src/utf16.h"
#include "common.h"

int
main()
{
	// Test if valid UTF-16 is correctly detected as valid.

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

	assert(utf16_valid(4, native));

	mem_set((uint8_t*)native, 0, 4);

	// Test that UTF-16 with an unpaired surrogate is correctly identified
	// as invalid.

	const uint8_t invalid1_big_endian[] = {0xd8, 0x3d};
	assert(binary_uint16_decode(sizeof(invalid1_big_endian),
			       invalid1_big_endian, native,
			       ENCODING_BYTE_ORDER_BIG) == 0);

	assert(!utf16_valid(ARRAY_SIZEOF(native), native));

	// Test that UTF-16 with a surrogate pair truncated by the length is
	// correctly identified as invalid.

	const uint8_t invalid2_big_endian[] = {0xd8, 0x3d};
	for (size_t i = 0; i < ARRAY_SIZEOF(invalid2_big_endian); i += 2) {
		assert(binary_uint16_decode(
				       ARRAY_SIZEOF(invalid2_big_endian) - i,
				       &invalid2_big_endian[i], &native[i / 2],
				       ENCODING_BYTE_ORDER_BIG) == 0);
	}

	assert(!utf16_valid(1, native));

	// Test that an invalid 16-bit value is correctly identified as
	// invalid.

	uint16_t invalid_val = 0xdc00;
	assert(!utf16_valid(1, &invalid_val));

	// Test that the values 0xffff and 0xfffe, which are valid in terms of
	// the encoding, but not valid Unicode codepoints, are correctly
	// detected as invalid.

	const uint16_t invalid_val2 = 0xffff;
	const uint16_t invalid_val3 = 0xfffe;

	assert(!utf16_valid(1, &invalid_val2));
	assert(!utf16_valid(1, &invalid_val3));

	// Test that an empty string is correctly identified as valid UTF-16.

	assert(utf16_valid(0, NULL));
	assert(utf16_valid(0, &invalid_val));

	// Test that utf16_valid correctly returns false when given
	// invalid parameters.

	assert(!utf16_valid(4, NULL));
}
