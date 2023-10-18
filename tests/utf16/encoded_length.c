// SPDX-License-Identifier: 0BSD
// Copyright (C) 2022 Ayman El Didi
#include <inttypes.h>

#include "../../utf16/utf16.h"
#include "common.h"

int
main()
{
	// Test that regular codepoints and surrogate pairs are correctly
	// counted as 1 and 2 codepoints respectively.

	const uint32_t grin_cp = 0x1f600;
	const uint32_t bom_cp  = 0xfeff;

	assert(utf16_encoded_length(1, &bom_cp) == 1);
	assert(utf16_encoded_length(1, &grin_cp) == 2);

	// Test that 0 is returned when str_len is 0.

	assert(utf16_encoded_length(0, NULL) == 0);
}
