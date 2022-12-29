// SPDX-License-Identifier: 0BSD
// Copyright (C) 2022 Ayman El Didi
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>

#include "../../src/hex.h"
#include "common.h"

int
main()
{
	assert(hex_encoded_length(16) == 32);
}
