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
	uint32_t buf[4] = {0};

	// Test if we can correctly decode various sized codepoints.

	const uint8_t  valid[] = "\x0a\xc2\xb6\xe2\x80\xb1\xf0\xa0\x9c\x8e";
	const uint32_t valid_decoded[] = {
			// \n NEWLINE (U+000A) 1 byte
			0x000a,
			// ¶ PILCROW SIGN (U+00B6) 2 bytes
			0x00b6,
			// ‱ PER TEN THOUSAND SIGN (U+2031) 3 bytes
			0x2031,
			// 𠜎 'to castrate a fowl, a capon' (U+2070E) 4 bytes
			0x2070e,
	};

	assert(utf8_decode(sizeof(valid) - 1, valid, ARRAY_SIZEOF(buf), buf) ==
			0);
	assert(buf[0] == valid_decoded[0] && buf[1] == valid_decoded[1] &&
			buf[2] == valid_decoded[2] &&
			buf[3] == valid_decoded[3]);

	mem_set((uint8_t*)buf, 0, 10);

	// Test if an invalid codepoint is correctly decoded as the Unicode
	// Replacement Character (U+FFFD).

	const uint8_t  invalid[]       = "\xff";
	const uint32_t invalid_decoded = ENCODING_CODEPOINT_ERROR;

	assert(utf8_decode(sizeof(invalid) - 1, invalid, ARRAY_SIZEOF(buf),
			       buf) == 0);
	assert(buf[0] == invalid_decoded);

	mem_set((uint8_t*)buf, 0, 10);

	// Test if passing str and buf as NULL returns
	// ENCODING_INVALID_NULL_POINTER.

	assert(utf8_decode(1, NULL, ARRAY_SIZEOF(buf), buf) ==
			ENCODING_INVALID_NULL_POINTER);
	assert(utf8_decode(1, valid, 1, NULL) ==
			ENCODING_INVALID_NULL_POINTER);
	assert(buf[0] == 0);

	mem_set((uint8_t*)buf, 0, 10);

	// Test if passing an empty string returns 0 and doesn't write anything
	// to out.

	assert(utf8_decode(0, NULL, 0, NULL) == 0);

	// Test if passing a length too small returns
	// ENCODING_BUFFER_TOO_SMALL.

	assert(utf8_decode(ARRAY_SIZEOF(valid) - 1, valid, 1, buf) ==
			ENCODING_BUFFER_TOO_SMALL);
	assert(buf[0] == 0);

	assert(utf8_decode(ARRAY_SIZEOF(valid) - 1, valid, 0, NULL) ==
			ENCODING_BUFFER_TOO_SMALL);
	assert(buf[0] == 0);

	// Test if passing an input length too small returns
	// ENCODING_INVALID_ARGUMENT.

	assert(utf8_decode(ARRAY_SIZEOF(valid) - 2, valid, ARRAY_SIZEOF(buf),
			       buf) == ENCODING_INVALID_ARGUMENT);
	assert(buf[0] == 0);
}
