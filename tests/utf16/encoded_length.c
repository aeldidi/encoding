// SPDX-License-Identifier: 0BSD
// Copyright (C) 2022 Ayman El Didi
#include <stdint.h>

#include "common.h"
#include "encoding/utf16.h"

int
main()
{
	size_t out = 0;

	// Test that regular codepoints and surrogate pairs are correctly
	// counted as 1 and 2 codepoints respectively.

	const uint32_t grin_cp = 0x1f600;
	const uint32_t bom_cp  = 0xfeff;

	assert(utf16_encoded_length(1, &bom_cp, &out) == 0);
	assert(out == 1);
	assert(utf16_encoded_length(1, &grin_cp, &out) == 0);
	assert(out == 2);

	// Test that 0 is returned when str_len is 0.

	assert(utf16_encoded_length(0, NULL, NULL) == 0);

	// Test that utf16_encoded_size correctly returns
	// -1 when passed invalid parameters.

	assert(utf16_encoded_length(1, NULL, &out) ==
			ENCODING_INVALID_NULL_POINTER);
	assert(utf16_encoded_length(1, &bom_cp, NULL) ==
			ENCODING_INVALID_NULL_POINTER);
	assert(utf16_encoded_length(1, NULL, NULL) ==
			ENCODING_INVALID_NULL_POINTER);
}
