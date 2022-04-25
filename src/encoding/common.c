// SPDX-License-Identifier: BSL-1.0
// Copyright (C) 2022 Ayman El Didi
#include "encoding/common.h"

static const char* error_descriptions[] = {
		"Success",
		"Invalid NULL pointer argument",
		"Invalid argument",
		"Length of output buffer too small",
		"Argument not power of 2 when required",
		"Not an error the encoding library can return",
};

const char*
encoding_strerror(const int error)
{
	if (error >= 0) {
		return error_descriptions[0];
	}

	if (error < -4) {
		return error_descriptions[5];
	}

	return error_descriptions[-error];
}
