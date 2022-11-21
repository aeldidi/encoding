// SPDX-License-Identifier: 0BSD
// Copyright (C) 2022 Ayman El Didi
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "encoding/common.h"
#include "encoding/utf8.h"

#include "compiler_extensions.h"

// The Unicode replacement character (U+FFFD) is sometimes used when invalid
// characters are encountered.
static const uint8_t utf8_replacement[] = {0xef, 0xbf, 0xbd};

// The encoded sizes in bytes of UTF-8 codepoints based on their first byte.
// Generated using tools/gen_utf8_sizes.c
static const size_t encoded_size_table[] = {
		//
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3,
		3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1,
		//
};

static size_t
utf8_codepoint_size(const uint32_t cp)
{
	if (cp <= 0x7F) {
		// ASCII
		return 1;
	} else if (cp <= 0x07FF) {
		// Two byte character
		return 2;
	} else if (cp <= 0xFFFF) {
		// Three byte character
		return 3;
	} else if (cp <= 0x10FFFF) {
		// Four byte character
		return 4;
	}

	return 0;
}

static bool
is_continuation(const uint8_t b)
{
	return b >= 0x80 && b <= 0xbf;
}

bool
utf8_valid(const size_t str_len, const uint8_t* str)
{
	if (unlikely(str == NULL)) {
		return false;
	}

	// Unicode Standard Version 14, Table 3-7. Well-Formed UTF-8 Byte
	// Sequences:
	//
	// +--------------------+--------+--------+--------+--------+
	// |   Code Points      | First  | Second | Third  | Fourth |
	// +--------------------+--------+--------+--------+--------+
	// | U+0000..U+007F     | 00..7F |        |        |        |
	// | U+0080..U+07FF     | C2..DF | 80..BF |        |        |
	// | U+0800..U+0FFF     | E0     | A0..BF | 80..BF |        |
	// | U+1000..U+CFFF     | E1..EC | 80..BF | 80..BF |        |
	// | U+D000..U+D7FF     | ED     | 80..9F | 80..BF |        |
	// | U+E000..U+FFFF     | EE..EF | 80..BF | 80..BF |        |
	// | U+10000..U+3FFFF   | F0     | 90..BF | 80..BF | 80..BF |
	// | U+40000..U+FFFFF   | F1..F3 | 80..BF | 80..BF | 80..BF |
	// | U+100000..U+10FFFF | F4     | 80..8F | 80..BF | 80..BF |
	// +--------------------+--------+--------+--------+--------+
	for (size_t i = 0; i < str_len; i += 1) {
		const uint8_t b = str[i];
		// As a consequence of the well-formedness conditions specified
		// in Table 3-7, the following byte values are disallowed in
		// UTF-8: C0–C1, F5–FF.
		if (b == 0xc0 || b == 0xc1 || b >= 0xf5) {
			return false;
		}

		if (b <= 0x7f) {
			continue;
		}

		if (b >= 0xc2 && b <= 0xdf) {
			if (i + 1 >= str_len || !is_continuation(str[i + 1])) {
				return false;
			}

			i += 1;
			continue;
		}

		if (b == 0xe0) {
			if (i + 2 >= str_len) {
				return false;
			}

			if (!(str[i + 1] >= 0xa0 && str[i + 1] <= 0xbf) ||
					!is_continuation(str[i + 2])) {
				return false;
			}

			i += 2;
			continue;
		}

		if (b >= 0xe1 && b <= 0xec) {
			if (i + 2 >= str_len) {
				return false;
			}

			if (!is_continuation(str[i + 1]) ||
					!is_continuation(str[i + 2])) {
				return false;
			}

			i += 2;
			continue;
		}

		if (b == 0xed) {
			if (i + 2 >= str_len) {
				return false;
			}

			if (!(str[i + 1] >= 0x80 && str[i + 1] <= 0x9f) ||
					!is_continuation(str[i + 2])) {
				return false;
			}

			i += 2;
			continue;
		}

		if (b >= 0xee && b <= 0xef) {
			if (i + 2 >= str_len) {
				return false;
			}

			if (!is_continuation(str[i + 1]) ||
					!is_continuation(str[i + 2])) {
				return false;
			}

			i += 2;
			continue;
		}

		if (b == 0xf0) {
			if (i + 3 >= str_len) {
				return false;
			}

			if (!(str[i + 1] >= 0x90 && str[i + 1] <= 0xbf) ||
					!is_continuation(str[i + 2]) ||
					!is_continuation(str[i + 3])) {
				return false;
			}

			i += 3;
			continue;
		}

		if (b >= 0xf1 && b <= 0xf3) {
			if (i + 3 >= str_len) {
				return false;
			}

			if (!is_continuation(str[i + 1]) ||
					!is_continuation(str[i + 2]) ||
					!is_continuation(str[i + 3])) {
				return false;
			}

			i += 3;
			continue;
		}

		if (b == 0xf4) {
			if (i + 3 >= str_len) {
				return false;
			}

			if (!(str[i + 1] >= 0x80 && str[i + 1] <= 0x8f) ||
					!is_continuation(str[i + 2]) ||
					!is_continuation(str[i + 3])) {
				return false;
			}

			i += 3;
			continue;
		}

		return false;
	}

	return true;
}

int
utf8_encoded_length(const size_t str_len, const uint32_t* str, size_t* out)
{
	if (out == NULL) {
		return ENCODING_INVALID_NULL_POINTER;
	}

	if (str_len == 0) {
		*out = 0;
		return 0;
	}

	if (str == NULL) {
		return ENCODING_INVALID_NULL_POINTER;
	}

	size_t result = 0;
	for (size_t i = 0; i < str_len; i += 1) {
		size_t size = utf8_codepoint_size(str[i]);
		if (size == 0) {
			size = 3;
		}

		result += size;
	}

	*out = result;
	return 0;
}

int
utf8_codepoint_encode(const uint32_t cp, const size_t out_len, uint8_t* out)
{
	if (out == NULL && out_len > 0) {
		return ENCODING_INVALID_NULL_POINTER;
	}

	size_t size = utf8_codepoint_size(cp);
	if (out_len < size) {
		return ENCODING_BUFFER_TOO_SMALL;
	}

	uint8_t* bytes = out;

	switch (size) {
	case 1:
		bytes[0] = (uint8_t)cp;
		break;
	case 2:
		bytes[0] = ((cp >> 6) & 0x1f) | 0xc0;
		bytes[1] = ((cp >> 0) & 0x3f) | 0x80;
		break;
	case 3:
		bytes[0] = ((cp >> 12) & 0x0f) | 0xe0;
		bytes[1] = ((cp >> 6) & 0x3f) | 0x80;
		bytes[2] = ((cp >> 0) & 0x3f) | 0x80;
		break;
	case 4:
		bytes[0] = ((cp >> 18) & 0x07) | 0xf0;
		bytes[1] = ((cp >> 12) & 0x3f) | 0x80;
		bytes[2] = ((cp >> 6) & 0x3f) | 0x80;
		bytes[3] = ((cp >> 0) & 0x3f) | 0x80;
		break;
	default:
		if (out_len < 3) {
			return ENCODING_BUFFER_TOO_SMALL;
		}

		bytes[0] = utf8_replacement[0];
		bytes[1] = utf8_replacement[1];
		bytes[2] = utf8_replacement[2];

		return 3;
	}

	return (int)size;
}

int
utf8_encode(const size_t str_len, const uint32_t* str, const size_t out_len,
		uint8_t* out)
{
	if (str_len == 0) {
		return 0;
	}

	if (str == NULL) {
		return ENCODING_INVALID_NULL_POINTER;
	}

	size_t encoded_size = 0;
	// We have already checked that str is not NULL and &encoded_size
	// can not be NULL, so there is no need to check the return value.
	(void)utf8_encoded_length(str_len, str, &encoded_size);

	if (encoded_size > out_len) {
		return ENCODING_BUFFER_TOO_SMALL;
	}

	if (out == NULL) {
		return ENCODING_INVALID_NULL_POINTER;
	}

	size_t j = 0;
	for (size_t i = 0; i < str_len; i += 1) {
		j += utf8_codepoint_encode(str[i], out_len - j, &out[j]);
	}

	return (int)j;
}

int
utf8_decoded_length(const size_t str_len, const uint8_t* str, size_t* out)
{
	if (unlikely(out == NULL)) {
		return ENCODING_INVALID_NULL_POINTER;
	}

	if (unlikely(str_len == 0)) {
		*out = 0;
		return 0;
	}

	if (unlikely(str == NULL)) {
		return ENCODING_INVALID_NULL_POINTER;
	}

	size_t result = 0;
	size_t i      = 0;
	while (i < str_len) {
		result += 1;
		i += encoded_size_table[str[i]];
	}

	*out = result;
	return 0;
}

uint32_t
utf8_codepoint_decode(const size_t str_len, const uint8_t* str, size_t* size)
{
	size_t  tmp;
	size_t* out_size = size;
	if (unlikely(size == NULL)) {
		out_size = &tmp;
	}

	if (unlikely(str == NULL || str_len == 0)) {
		*out_size = 3;
		return ENCODING_CODEPOINT_ERROR;
	}

	const size_t len = encoded_size_table[str[0]];
	if (unlikely(str_len < len)) {
		*out_size = 3;
		return ENCODING_CODEPOINT_ERROR;
	}

	switch (len) {
	case 2:
		if (!is_continuation(str[1])) {
			*out_size = 3;
			return ENCODING_CODEPOINT_ERROR;
		}

		*out_size = 2;
		return ((0x1f & str[0]) << 6) | (0x3f & str[1]);
	case 3: {
		const bool ranges = (str[1] >= 0xa0 && str[1] <= 0xbf) ||
				    (str[1] >= 0x80 && str[1] <= 0x9f);
		if (!is_continuation(str[2]) || !ranges) {
			*out_size = 3;
			return ENCODING_CODEPOINT_ERROR;
		}

		*out_size = 3;
		return ((0x0f & str[0]) << 12) | ((0x3f & str[1]) << 6) |
		       (0x3f & str[2]);
	}
	case 4: {
		const bool byte2checks = is_continuation(str[1]) ||
					 (str[1] >= 0x90 && str[1] <= 0xbf) ||
					 (str[1] >= 0x80 && str[1] <= 0x8f);
		if (!is_continuation(str[2]) || !is_continuation(str[3]) ||
				!byte2checks) {
			*out_size = 3;
			return ENCODING_CODEPOINT_ERROR;
		}

		*out_size = 4;
		return ((0x07 & str[0]) << 18) | ((0x3f & str[1]) << 12) |
		       ((0x3f & str[2]) << 6) | (0x3f & str[3]);
	}
	default:
		break;
	}

	// Invalid bytes are 1 in encoded_size_table, so we still have
	// to check for correctness here.
	//
	// In every other case, the first byte can be assumed valid.
	if (str[0] >= 0x7f) {
		*out_size = 3;
		return ENCODING_CODEPOINT_ERROR;
	}

	*out_size = 1;
	return str[0];
}

int
utf8_decode(const size_t str_len, const uint8_t* str, const size_t out_len,
		uint32_t* out)
{
	if (str_len == 0) {
		return 0;
	}

	if (str == NULL) {
		return ENCODING_INVALID_NULL_POINTER;
	}

	if (out == NULL) {
		if (out_len == 0) {
			return ENCODING_BUFFER_TOO_SMALL;
		}

		return ENCODING_INVALID_NULL_POINTER;
	}

	bool   truncated = false;
	size_t j         = 0;
	size_t i         = 0;
	while (i < str_len && j < out_len) {
		size_t len = encoded_size_table[str[i]];
		if (unlikely(i + len > str_len)) {
			truncated = true;
			break;
		}

		out[j] = utf8_codepoint_decode(str_len - i, str + i, &len);
		i += len;
		j += 1;
	}

	// We couldn't decode the entire output.
	if (unlikely(i < str_len)) {
		// We don't want to partially write to out if we couldn't
		// decode the entire buffer.
		for (size_t k = 0; k < out_len; k += 1) {
			out[k] = 0;
		}

		// A codepoint in the input was truncated.
		if (truncated) {
			return ENCODING_INVALID_ARGUMENT;
		}

		return ENCODING_BUFFER_TOO_SMALL;
	}

	return 0;
}
