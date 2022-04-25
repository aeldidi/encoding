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
	// Test if we can decode various different sizes of codepoints from raw
	// UTF-8.

	size_t size = 0;

	// \n NEWLINE (U+000A)
	const uint8_t  newline[]  = "\x0a";
	const uint32_t newline_cp = 0x000a;
	assert(utf8_codepoint_decode(ARRAY_SIZEOF(newline) - 1, newline,
			       NULL) == newline_cp);
	assert(utf8_codepoint_decode(ARRAY_SIZEOF(newline) - 1, newline,
			       &size) == newline_cp);
	assert(size == 1);

	// ¶ PILCROW SIGN (U+00B6)
	const uint8_t  pilcrow[]  = "\xc2\xb6";
	const uint32_t pilcrow_cp = 0x00b6;
	assert(utf8_codepoint_decode(ARRAY_SIZEOF(pilcrow) - 1, pilcrow,
			       NULL) == pilcrow_cp);
	assert(utf8_codepoint_decode(ARRAY_SIZEOF(pilcrow) - 1, pilcrow,
			       &size) == pilcrow_cp);
	assert(size == 2);

	// ‱ PER TEN THOUSAND SIGN (U+2031)
	const uint8_t  ten_thous[]  = "\xe2\x80\xb1";
	const uint32_t ten_thous_cp = 0x2031;
	assert(utf8_codepoint_decode(ARRAY_SIZEOF(ten_thous) - 1, ten_thous,
			       NULL) == ten_thous_cp);
	assert(utf8_codepoint_decode(ARRAY_SIZEOF(ten_thous) - 1, ten_thous,
			       &size) == ten_thous_cp);
	assert(size == 3);

	// 𠜎 'to castrate a fowl, a capon' (U+2070E)
	const uint8_t  fowl[]  = "\xf0\xa0\x9c\x8e";
	const uint32_t fowl_cp = 0x2070e;
	assert(utf8_codepoint_decode(ARRAY_SIZEOF(fowl) - 1, fowl, NULL) ==
			fowl_cp);
	assert(utf8_codepoint_decode(ARRAY_SIZEOF(fowl) - 1, fowl, &size) ==
			fowl_cp);
	assert(size == 4);

	// Test that an invalid 4 byte codepoint with correct upper bits is
	// detected as so.

	size = 0;

	const uint8_t utf8_4_byte_char_invalid[] = "\xf7\x80\x80\x80";
	assert(utf8_codepoint_decode(
			       ARRAY_SIZEOF(utf8_4_byte_char_invalid) - 1,
			       utf8_4_byte_char_invalid,
			       NULL) == ENCODING_CODEPOINT_ERROR);
	assert(utf8_codepoint_decode(
			       ARRAY_SIZEOF(utf8_4_byte_char_invalid) - 1,
			       utf8_4_byte_char_invalid,
			       &size) == ENCODING_CODEPOINT_ERROR);
	assert(size == 3); // The Unicode Replacement Character is 3 bytes
			   // wide.

	// Test if ENCODING_CODEPOINT_ERROR is returned when a multibyte
	// codepoint is truncated in the input.

	assert(utf8_codepoint_decode(ARRAY_SIZEOF(fowl) - 2, fowl, NULL) ==
			ENCODING_CODEPOINT_ERROR);
	assert(utf8_codepoint_decode(ARRAY_SIZEOF(fowl) - 2, fowl, &size) ==
			ENCODING_CODEPOINT_ERROR);
	assert(size == 3);

	// Test if utf8_codepoint_decode returns
	// ENCODING_CODEPOINT_ERROR for invalid UTF-8.

	size = 0;

	const uint8_t invalid_utf8_byte[] = "\xff\xff";
	assert(utf8_codepoint_decode(ARRAY_SIZEOF(invalid_utf8_byte) - 1,
			       invalid_utf8_byte,
			       NULL) == ENCODING_CODEPOINT_ERROR);
	assert(utf8_codepoint_decode(ARRAY_SIZEOF(invalid_utf8_byte) - 1,
			       invalid_utf8_byte,
			       &size) == ENCODING_CODEPOINT_ERROR);
	assert(size == 3);

	// Test if utf8_codepoint_decode returns
	// ENCODING_CODEPOINT_ERROR when str is NULL.

	size = 0;

	assert(utf8_codepoint_decode(0, invalid_utf8_byte, NULL) ==
			ENCODING_CODEPOINT_ERROR);
	assert(utf8_codepoint_decode(0, invalid_utf8_byte, &size) ==
			ENCODING_CODEPOINT_ERROR);
	assert(size == 3);
	assert(utf8_codepoint_decode(1, NULL, NULL) ==
			ENCODING_CODEPOINT_ERROR);
	assert(utf8_codepoint_decode(1, NULL, &size) ==
			ENCODING_CODEPOINT_ERROR);
	assert(size == 3);
}
