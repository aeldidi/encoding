// SPDX-License-Identifier: 0BSD
// Copyright (C) 2022 Ayman El Didi
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "common.h"
#include "encoding/utf8.h"

int
main()
{
	// Test if utf8_decoded_length returns
	// ENCODING_INVALID_NULL_POINTER if str or out is NULL.

	size_t        out     = 0;
	const uint8_t ascii[] = "coolio";

	assert(utf8_decoded_length(1, NULL, &out) ==
			ENCODING_INVALID_NULL_POINTER);
	assert(utf8_decoded_length(1, ascii, NULL) ==
			ENCODING_INVALID_NULL_POINTER);

	// Test if utf8_decoded_length gives 0 when len is 0.
	assert(utf8_decoded_length(0, ascii, &out) == 0);
	assert(out == 0);

	// Test if a string of all valid codepoints of different sizes returns
	// the proper number.

	// "\n¶‱𠜎" == NEWLINE (U+000A), PILCROW SIGN (U+00B6),
	//              PER TEN THOUSAND SIGN (U+2031),
	//              'to castrate a fowl, a capon' (U+2070E)
	const uint8_t test[] = "\x0a\xc2\xb6\xe2\x80\xb1\xf0\xa0\x9c\x8e";
	assert(utf8_decoded_length(ARRAY_SIZEOF(test) - 1, test, &out) == 0);
	assert(out == 4);

	// "\n¶‱𠜎" == NEWLINE (U+000A), PILCROW SIGN (U+00B6),
	//              4 invalid bytes (\0xff),
	//              'to castrate a fowl, a capon' (U+2070E)
	const uint8_t test2[] = "\x0a\xc2\xb6\xff\xff\xff\xff\xf0\xa0\x9c\x8e";
	assert(utf8_decoded_length(ARRAY_SIZEOF(test2) - 1, test2, &out) == 0);
	assert(out == 7);
}
