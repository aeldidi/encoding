// SPDX-License-Identifier: BSL-1.0
// Copyright (C) 2022 Ayman El Didi
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "common.h"
#include "encoding/utf8.h"

int
main()
{
	// Test if utf8_encoded_length returns
	// ENCODING_INVALID_NULL_POINTER if str or out is NULL.

	size_t         out     = 0;
	const uint32_t ascii[] = {
			0x0063, // c
			0x006f, // o
			0x006f, // o
			0x006c, // l
			0x0069, // i
			0x006f, // o
	};

	assert(utf8_encoded_length(1, NULL, &out) ==
			ENCODING_INVALID_NULL_POINTER);
	assert(utf8_encoded_length(1, ascii, NULL) ==
			ENCODING_INVALID_NULL_POINTER);

	// Test if utf8_encoded_length gives 0 when len is 0.
	assert(utf8_encoded_length(0, ascii, &out) == 0);
	assert(out == 0);

	// Test if a string of all valid codepoints of different sizes returns
	// the proper number.

	const uint32_t test[] = {
			0x000a,  // NEWLINE (U+000A)
			0x00b6,  // PILCROW SIGN (U+00B6)
			0x2031,  // PER TEN THOUSAND SIGN (U+2031)
			0x2070e, // 'to castrate a fowl, a capon' (U+2070E)
	};
	assert(utf8_encoded_length(ARRAY_SIZEOF(test), test, &out) == 0);
	assert(out == 10);

	// "\n¶‱𠜎" == NEWLINE (U+000A), PILCROW SIGN (U+00B6),
	//              4 invalid codepoints,
	//              'to castrate a fowl, a capon' (U+2070E)
	const uint32_t test2[] = {
			0x000a,     // NEWLINE (U+000A)
			0x00b6,     // PILCROW SIGN (U+00B6)
			0xffffffff, // invalid codepoint
			0xffffffff, // invalid codepoint
			0xffffffff, // invalid codepoint
			0xffffffff, // invalid codepoint
			0x2070e,    // 'to castrate a fowl, a capon' (U+2070E)
	};
	assert(utf8_encoded_length(ARRAY_SIZEOF(test2), test2, &out) == 0);
	assert(out == 19);
}
