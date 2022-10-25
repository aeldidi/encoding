// SPDX-License-Identifier: 0BSD
// Copyright (C) 2022 Ayman El Didi
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "common.h"
#include "encoding/hex.h"

int
main()
{
	assert(hex_encoded_length(16) == 32);
}
