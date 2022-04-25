// SPDX-License-Identifier: BSL-1.0
// Copyright (C) 2022 Ayman El Didi
#include <stddef.h>
#include <stdint.h>

#include "common.h"
#include "encoding/base32.h"

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