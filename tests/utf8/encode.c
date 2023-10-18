// SPDX-License-Identifier: 0BSD
// Copyright (C) 2022 Ayman El Didi
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>

#include "../../utf8/utf8.h"
#include "common.h"

int
main()
{
	uint8_t buf[10] = {0};

	// Test if we can correctly encode various sized codepoints.

	const uint32_t valid[] = {
			// \n NEWLINE (U+000A) 1 byte
			0x000a,
			// ¶ PILCROW SIGN (U+00B6) 2 bytes
			0x00b6,
			// ‱ PER TEN THOUSAND SIGN (U+2031) 3 bytes
			0x2031,
			// 𠜎 'to castrate a fowl, a capon' (U+2070E) 4 bytes
			0x2070e,
	};
	const uint8_t valid_encoded[] =
			"\x0a\xc2\xb6\xe2\x80\xb1\xf0\xa0\x9c\x8e";

	assert(utf8_encode(ARRAY_SIZEOF(valid), valid, ARRAY_SIZEOF(buf),
			       buf) == 10);
	assert(mem_equal(buf, valid_encoded, sizeof(valid_encoded) - 1));

	mem_set(buf, 0, sizeof(buf));

	// Test if an invalid codepoint is correctly encoded as the Unicode
	// Replacement Character (U+FFFD).

	const uint32_t invalid[]         = {0xffffffff};
	const uint8_t  invalid_encoded[] = "\xef\xbf\xbd";

	assert(utf8_encode(1, invalid, ARRAY_SIZEOF(buf), buf) == 3);
	assert(mem_equal(buf, invalid_encoded, sizeof(invalid_encoded) - 1));

	mem_set(buf, 0, sizeof(buf));

	// Test if passing an empty string returns 0 and doesn't write to out
	// at all.

	assert(utf8_encode(0, NULL, 0, NULL) == 0);

	// Test if passing a length too small returns
	// ENCODING_BUFFER_TOO_SMALL.

	assert(utf8_encode(ARRAY_SIZEOF(valid), valid, 1, buf) ==
			ENCODING_BUFFER_TOO_SMALL);
	assert(buf[0] == 0);
}
