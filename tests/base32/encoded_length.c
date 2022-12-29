// SPDX-License-Identifier: 0BSD
// Copyright (C) 2022 Ayman El Didi
#include <inttypes.h>
#include <stddef.h>

#include "../../src/base32.h"
#include "common.h"

int
main()
{
	assert(base32_encoded_length(0) == 0);
	assert(base32_encoded_length(1) == 8);
	assert(base32_encoded_length(2) == 8);
	assert(base32_encoded_length(3) == 8);
	assert(base32_encoded_length(4) == 8);
	assert(base32_encoded_length(5) == 8);
	assert(base32_encoded_length(6) == 16);
}
