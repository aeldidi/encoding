// SPDX-License-Identifier: 0BSD
// Copyright (C) 2022 Ayman El Didi
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>

#include "../../src/utf8.h"
#include "common.h"

int
main()
{
	const uint32_t ascii[] = {
			0x0063, // c
			0x006f, // o
			0x006f, // o
			0x006c, // l
			0x0069, // i
			0x006f, // o
	};

	// Test if utf8_encoded_length gives 0 when len is 0.
	assert(utf8_encoded_length(0, ascii) == 0);

	// Test if a string of all valid codepoints of different sizes returns
	// the proper number.

	const uint32_t test[] = {
			0x000a,  // NEWLINE (U+000A)
			0x00b6,  // PILCROW SIGN (U+00B6)
			0x2031,  // PER TEN THOUSAND SIGN (U+2031)
			0x2070e, // 'to castrate a fowl, a capon' (U+2070E)
	};
	assert(utf8_encoded_length(ARRAY_SIZEOF(test), test) == 10);

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
	assert(utf8_encoded_length(ARRAY_SIZEOF(test2), test2) == 19);
}
