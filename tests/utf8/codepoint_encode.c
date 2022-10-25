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
	uint8_t buf[4] = {0};

	// Test if we can correctly encode various sized codepoints.

	// \n NEWLINE (U+000A)
	const uint8_t  newline[]  = "\x0a";
	const uint32_t newline_cp = 0x000a;

	assert(utf8_codepoint_encode(newline_cp, ARRAY_SIZEOF(buf), buf) == 1);
	assert(buf[0] == newline[0] && buf[1] == 0 && buf[2] == 0 &&
			buf[3] == 0);

	mem_set(buf, 0, sizeof(buf));

	// ¶ PILCROW SIGN (U+00B6)
	const uint8_t  pilcrow[]  = "\xc2\xb6";
	const uint32_t pilcrow_cp = 0x00b6;

	assert(utf8_codepoint_encode(pilcrow_cp, ARRAY_SIZEOF(buf), buf) == 2);
	assert(mem_equal(buf, pilcrow, 2));

	mem_set(buf, 0, sizeof(buf));

	// ‱ PER TEN THOUSAND SIGN (U+2031)
	const uint8_t  ten_thous[]  = "\xe2\x80\xb1";
	const uint32_t ten_thous_cp = 0x2031;

	assert(utf8_codepoint_encode(ten_thous_cp, ARRAY_SIZEOF(buf), buf) ==
			3);
	assert(mem_equal(buf, ten_thous, 3));

	mem_set(buf, 0, sizeof(buf));

	// 𠜎 'to castrate a fowl, a capon' (U+2070E)
	const uint8_t  fowl[]  = "\xf0\xa0\x9c\x8e";
	const uint32_t fowl_cp = 0x2070e;

	assert(utf8_codepoint_encode(fowl_cp, ARRAY_SIZEOF(buf), buf) == 4);
	assert(mem_equal(buf, fowl, ARRAY_SIZEOF(buf)));

	mem_set(buf, 0, sizeof(buf));

	// Test if utf8_codepoint_encode encodes a Unicode Replacement
	// Character (U+FFFD) when passed an invalid codepoint.

	const uint8_t replacement[] = "\xef\xbf\xbd";

	assert(utf8_codepoint_encode(0xffffffff, ARRAY_SIZEOF(buf), buf) == 3);
	assert(mem_equal((uint8_t*)buf, replacement, 3));

	mem_set(buf, 0, sizeof(buf));

	// Test if utf8_codepoint_encode returns
	// ENCODING_BUFFER_TOO_SMALL when there's not enough room to encode the
	// codepoint.

	assert(utf8_codepoint_encode(pilcrow_cp, 1, buf) ==
			ENCODING_BUFFER_TOO_SMALL);
	assert(buf[0] == 0 && buf[1] == 0 && buf[2] == 0 && buf[3] == 0);

	assert(utf8_codepoint_encode(0xffffffff, 2, buf) ==
			ENCODING_BUFFER_TOO_SMALL);
	assert(buf[0] == 0 && buf[1] == 0 && buf[2] == 0 && buf[3] == 0);

	// Test if utf8_codepoint_encode correctly returns
	// ENCODING_BUFFER_TOO_SMALL when the buffer size is 0.

	assert(utf8_codepoint_encode(newline_cp, 0, NULL) ==
			ENCODING_BUFFER_TOO_SMALL);
	assert(utf8_codepoint_encode(pilcrow_cp, 0, NULL) ==
			ENCODING_BUFFER_TOO_SMALL);
	assert(utf8_codepoint_encode(ten_thous_cp, 0, NULL) ==
			ENCODING_BUFFER_TOO_SMALL);
	assert(utf8_codepoint_encode(fowl_cp, 0, NULL) ==
			ENCODING_BUFFER_TOO_SMALL);

	// Test if utf8_codepoint_encode returns
	// ENCODING_INVALID_NULL_POINTER when given invalid NULL pointers.

	assert(utf8_codepoint_encode(0x0, 2, NULL) ==
			ENCODING_INVALID_NULL_POINTER);
	assert(buf[0] == 0 && buf[1] == 0 && buf[2] == 0 && buf[3] == 0);
}
