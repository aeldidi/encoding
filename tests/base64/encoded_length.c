// SPDX-License-Identifier: BSL-1.0
// Copyright (C) 2022 Ayman El Didi
#include <stddef.h>
#include <stdint.h>

#include "common.h"
#include "encoding/base64.h"

int
main()
{
	assert(base64_encoded_length(0) == 0);
	assert(base64_encoded_length(1) == 4);
	assert(base64_encoded_length(2) == 4);
	assert(base64_encoded_length(3) == 4);
	assert(base64_encoded_length(4) == 8);
	assert(base64_encoded_length(5) == 8);
	assert(base64_encoded_length(6) == 8);
}
