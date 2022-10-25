// SPDX-License-Identifier: 0BSD
// Copyright (C) 2022 Ayman El Didi
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "common.h"
#include "encoding/utf8.h"

// Used as a reference for different things that can be encountered in
// invalid UTF-8:
// https://www.cl.cam.ac.uk/~mgk25/ucs/examples/UTF-8-test.txt
int
main()
{
	// Test if valid UTF-8 is correctly detected as valid.
	// Includes a test for each range in Table 3-7 in
	// https://www.unicode.org/versions/Unicode14.0.0/ch03.pdf, page 56.

	// U+0000..U+007F
	const uint8_t valid1[] = "\x00\x7f";
	assert(utf8_valid(sizeof(valid1) - 1, valid1));

	// U+0080..U+07FF
	const uint8_t valid2[] = "\xc2\x80\xdf\xbf";
	assert(utf8_valid(sizeof(valid2) - 1, valid2));
	assert(!utf8_valid(sizeof(valid2) - 2, valid2));

	const uint8_t invalid2[] = "\xc2\x00\xdf\x00";
	assert(!utf8_valid(sizeof(invalid2) - 1, invalid2));

	// U+0800..U+0FFF
	const uint8_t valid3[] = "\xe0\xa0\x80\xe0\xbf\xbf";
	assert(utf8_valid(sizeof(valid3) - 1, valid3));
	assert(!utf8_valid(sizeof(valid3) - 2, valid3));

	const uint8_t invalid3[] = "\xe0\xa0\x00\xe0\xbf\x00";
	assert(!utf8_valid(sizeof(invalid3) - 1, invalid3));

	// U+1000..U+CFFF
	const uint8_t valid4[] = "\xe1\x80\x80\xec\xbf\xbf";
	assert(utf8_valid(sizeof(valid4) - 1, valid4));
	assert(!utf8_valid(sizeof(valid4) - 2, valid4));

	const uint8_t invalid4_1[] = "\xe1\x00\x80\xec\x00\xbf";
	assert(!utf8_valid(sizeof(invalid4_1) - 1, invalid4_1));

	const uint8_t invalid4_2[] = "\xe1\x80\x00\xec\xbf\x00";
	assert(!utf8_valid(sizeof(invalid4_2) - 1, invalid4_2));

	// U+D000..U+D7FF
	const uint8_t valid5[] = "\xed\x80\x80\xed\x9f\xbf";
	assert(utf8_valid(sizeof(valid5) - 1, valid5));
	assert(!utf8_valid(sizeof(valid5) - 2, valid5));

	const uint8_t invalid5_1[] = "\xed\x00\x80\xed\x00\xbf";
	assert(!utf8_valid(sizeof(invalid5_1) - 1, invalid5_1));

	const uint8_t invalid5_2[] = "\xed\x80\x00\xed\x9f\x00";
	assert(!utf8_valid(sizeof(invalid5_2) - 1, invalid5_2));

	// U+E000..U+FFFF
	const uint8_t valid6[] = "\xee\x80\x80\xef\xbf\xbf";
	assert(utf8_valid(sizeof(valid6) - 1, valid6));
	assert(!utf8_valid(sizeof(valid6) - 2, valid6));

	const uint8_t invalid6_1[] = "\xee\x00\x80\xef\x00\xbf";
	assert(!utf8_valid(sizeof(invalid6_1) - 1, invalid6_1));

	const uint8_t invalid6_2[] = "\xee\x80\x00\xef\xbf\x00";
	assert(!utf8_valid(sizeof(invalid6_2) - 1, invalid6_2));

	// U+10000..U+3FFFF
	const uint8_t valid7[] = "\xf0\x90\x80\x80\xf0\xbf\xbf\xbf";
	assert(utf8_valid(sizeof(valid7) - 1, valid7));
	assert(!utf8_valid(sizeof(valid7) - 2, valid7));

	const uint8_t invalid7_1[] = "\xf0\x00\x80\x80";
	assert(!utf8_valid(sizeof(invalid7_1) - 1, invalid7_1));

	const uint8_t invalid7_2[] = "\xf0\x90\x00\x80";
	assert(!utf8_valid(sizeof(invalid7_2) - 1, invalid7_2));

	const uint8_t invalid7_3[] = "\xf0\x90\x80\x00";
	assert(!utf8_valid(sizeof(invalid7_3) - 1, invalid7_3));

	// U+40000..U+FFFFF
	const uint8_t valid8[] = "\xf1\x80\x80\x80\xf3\xbf\xbf\xbf";
	assert(utf8_valid(sizeof(valid8) - 1, valid8));
	assert(!utf8_valid(sizeof(valid8) - 2, valid8));

	const uint8_t invalid8_1[] = "\xf1\x00\x80\x80";
	assert(!utf8_valid(sizeof(invalid8_1) - 1, invalid8_1));

	const uint8_t invalid8_2[] = "\xf1\x80\x00\x80";
	assert(!utf8_valid(sizeof(invalid8_2) - 1, invalid8_2));

	const uint8_t invalid8_3[] = "\xf1\x80\x80\x00";
	assert(!utf8_valid(sizeof(invalid8_3) - 1, invalid8_3));

	// U+100000..U+10FFFF
	const uint8_t valid9[] = "\xf4\x80\x80\x80\xf4\x8f\xbf\xbf";
	assert(utf8_valid(sizeof(valid9) - 1, valid9));
	assert(!utf8_valid(sizeof(valid9) - 2, valid9));

	const uint8_t invalid9_1[] = "\xf4\x00\x80\x80";
	assert(!utf8_valid(sizeof(invalid9_1) - 1, invalid9_1));

	const uint8_t invalid9_2[] = "\xf4\x80\x00\x80";
	assert(!utf8_valid(sizeof(invalid9_2) - 1, invalid9_2));

	const uint8_t invalid9_3[] = "\xf4\x80\x80\x00";
	assert(!utf8_valid(sizeof(invalid9_3) - 1, invalid9_3));

	// Test if all ASCII UTF-8 is correctly detected as valid.

	// "abc"
	const uint8_t valid_ascii[] = "\x61\x62\x63";
	assert(utf8_valid(sizeof(valid_ascii) - 1, valid_ascii));

	// "0123456789" (tested to check the fast path)
	const uint8_t valid_ascii2[] =
			"\x30\x31\x32\x33\x34\x35\x36\x37\x38\x39";
	assert(utf8_valid(sizeof(valid_ascii2) - 1, valid_ascii2));

	// Test if UTF-8 with overlong encoded characters is correctly detected
	// as invalid.
	//
	// These are all the character '/' (SOLIDUS (U+002F)) encoded overlong
	// in various ways.

	const uint8_t invalid_overlong1[] = "\xc0\xaf";
	assert(!utf8_valid(sizeof(invalid_overlong1) - 1, invalid_overlong1));

	const uint8_t invalid_overlong2[] = "\xe0\x80\xaf";
	assert(!utf8_valid(sizeof(invalid_overlong2) - 1, invalid_overlong2));

	const uint8_t invalid_overlong3[] = "\xf0\x80\x80\xaf";
	assert(!utf8_valid(sizeof(invalid_overlong3) - 1, invalid_overlong3));

	// Test if UTF-8 with stray continuation byte is correctly detected as
	// invalid.

	// NEWLINE (U+000A), continuation byte 0b10000000.
	const uint8_t invalid_continuation[] = "\x0a\x80";
	assert(!utf8_valid(sizeof(invalid_continuation) - 1,
			invalid_continuation));

	// Test if UTF-8 with sequence start bytes without continuations are
	// correctly detected as invalid.

	// 2 byte start without continuation:
	// 2 byte sequence start 0b11000000, NEWLINE (U+0x000A)
	const uint8_t invalid_start_nocont[] = "\xc0\x0a";
	assert(!utf8_valid(sizeof(invalid_start_nocont) - 1,
			invalid_start_nocont));

	// 3 byte start without continuation:
	// 3 byte sequence start 0b11100000, NEWLINE (U+0x000A)
	const uint8_t invalid_start_nocont2[] = "\xe0\x0a";
	assert(!utf8_valid(sizeof(invalid_start_nocont2) - 1,
			invalid_start_nocont2));

	// 4 byte start without continuation:
	// 4 byte sequence start 0b11110000, NEWLINE (U+0x000A)
	const uint8_t invalid_start_nocont3[] = "\xf0\x0a";
	assert(!utf8_valid(sizeof(invalid_start_nocont3) - 1,
			invalid_start_nocont3));

	// Test if UTF-8 with invalid bytes (0xfe and 0xff) are correctly
	// detected as invalid.

	const uint8_t fe[] = "\xfe";
	assert(!utf8_valid(sizeof(fe) - 1, fe));

	const uint8_t ff[] = "\xff";
	assert(!utf8_valid(sizeof(ff) - 1, ff));

	// NEWLINE (U+000A), invalid 0xfe
	const uint8_t fe_end[] = "\x0a\xfe";
	assert(!utf8_valid(sizeof(fe_end) - 1, fe_end));

	// NEWLINE (U+000A), invalid 0xff
	const uint8_t ff_end[] = "\x0a\xff";
	assert(!utf8_valid(sizeof(ff_end) - 1, ff_end));

	// Test if empty string is correctly detected as valid UTF-8.

	const uint8_t empty[] = "";
	assert(utf8_valid(0, empty));

	// Test if utf8_valid returns false when str is NULL.

	assert(!utf8_valid(0, NULL));
}
