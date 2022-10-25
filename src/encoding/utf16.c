// SPDX-License-Identifier: 0BSD
// Copyright (C) 2022 Ayman El Didi
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "encoding/utf16.h"

static bool
in_range16(const uint16_t x, const uint16_t start, const uint16_t end)
{
	return x >= start && x <= end;
}

bool
utf16_valid(const size_t str_len, const uint16_t* str)
{
	if (str == NULL && str_len > 0) {
		return false;
	}

	for (size_t i = 0; i < str_len; i += 1) {
		// 0xffff and 0xfffe are not actually Unicode Codepoints.
		if (str[i] == 0xffff || str[i] == 0xfffe) {
			return false;
		} else if (str[i] < 0xd800 || str[i] > 0xdfff) {
			continue;
		} else if (i + 1 < str_len ||
				in_range16(str[i], 0xd800, 0xdbff)) {
			if (!in_range16(str[i + 1], 0xdc00, 0xdfff)) {
				return false;
			}

			i += 1;
			break;
		}

		return false;
	}

	return true;
}

// Assumes input codepoint is valid.
static size_t
utf16_codepoint_encoded_size(const uint32_t cp)
{
	if (cp > 0x10ffff || cp == 0xffff || cp == 0xfffe ||
			in_range16(cp, 0xd800, 0xdfff)) {
		return 0;
	} else if (cp < 0x10000) {
		return 1;
	}

	return 2;
}

// differs from utf16_codepoint_encoded_size because this one returns
// 1 on error instead of 0.
static size_t
utf16_encoded_length_codepoint_size(const uint32_t cp)
{
	if (cp < 0x10000) {
		return 1;
	}

	return 2;
}

int
utf16_encoded_length(const size_t str_len, const uint32_t* str, size_t* out)
{
	if (str_len == 0) {
		return 0;
	}

	if (str == NULL || out == NULL) {
		return ENCODING_INVALID_NULL_POINTER;
	}

	size_t result = 0;
	for (size_t i = 0; i < str_len; i += 1) {
		result += utf16_encoded_length_codepoint_size(str[i]);
	}

	*out = result;
	return 0;
}

int
utf16_codepoint_encode(const uint32_t cp, const size_t out_len, uint16_t* out)
{
	if (out_len == 0) {
		return ENCODING_BUFFER_TOO_SMALL;
	}

	if (out == NULL) {
		return ENCODING_INVALID_NULL_POINTER;
	}

	size_t size = utf16_codepoint_encoded_size(cp);
	if (out_len < size) {
		return ENCODING_BUFFER_TOO_SMALL;
	}

	switch (size) {
	case 1:
		out[0] = (uint16_t)cp;
		break;
	case 2: {
		const uint32_t bits_10_to_19_set = 0xffc00;
		const uint32_t bits_0_to_9_set   = 0x003ff;
		const uint32_t u                 = cp - 0x10000;

		out[0] = 0xd800 | ((u & bits_10_to_19_set) >> 10);
		out[1] = 0xdc00 | ((u & bits_0_to_9_set) >> 0);
		break;
	}
	default:
		out[0] = (uint16_t)ENCODING_CODEPOINT_ERROR;
		break;
	}

	return 0;
}

uint32_t
utf16_codepoint_decode(const size_t str_len, const uint16_t* str, size_t* size)
{
	if (size != NULL) {
		*size = 1;
	}

	if (str == NULL || str_len == 0 || str[0] == 0xffff ||
			str[0] == 0xfffe) {
		return ENCODING_CODEPOINT_ERROR;
	}

	if (str[0] < 0xd800 || str[0] > 0xdfff) {
		return str[0];
	} else if (in_range16(str[0], 0xd800, 0xdbff)) {
		if (str_len == 1 || !in_range16(str[1], 0xdc00, 0xdfff)) {
			return ENCODING_CODEPOINT_ERROR;
		}

		if (size != NULL) {
			*size = 2;
		}

		uint32_t tmp = ((uint32_t)str[0] & 0x3ff) << 10 |
			       ((uint32_t)str[1] & 0x3ff) << 0;
		return tmp + 0x10000;
	} else {
		return ENCODING_CODEPOINT_ERROR;
	}
}

int
utf16_decoded_length(const size_t str_len, const uint16_t* str, size_t* out)
{
	if (str_len == 0) {
		return 0;
	}

	if (str == NULL || out == NULL) {
		return ENCODING_INVALID_NULL_POINTER;
	}

	size_t result = 0;
	for (size_t i = 0; i < str_len; i += 1) {
		if (in_range16(str[i], 0xd800, 0xdbff) && i + 1 < str_len &&
				in_range16(str[i + 1], 0xdc00, 0xdfff)) {
			i += 1;
		}

		result += 1;
	}

	*out = result;
	return 0;
}

int
utf16_encode(const size_t str_len, const uint32_t* str, const size_t out_len,
		uint16_t* out)
{
	if (str_len == 0) {
		return 0;
	}

	if (str == NULL || out == NULL) {
		return ENCODING_INVALID_NULL_POINTER;
	}

	size_t encoded_size = 0;
	// We have already checked that str is not NULL and &encoded_size
	// can not be NULL, so there is no need to check the return value.
	(void)utf16_encoded_length(str_len, str, &encoded_size);

	if (encoded_size > out_len) {
		return ENCODING_BUFFER_TOO_SMALL;
	}

	size_t j = 0;
	for (size_t i = 0; i < str_len; i += 1) {
		// We use SIZE_MAX here because we know the encoded size can
		// fit in the length specified as out_len.
		j += utf16_codepoint_encode(str[i], out_len - j, &out[j]);
	}

	return 0;
}

int
utf16_decode(const size_t str_len, const uint16_t* str, const size_t out_len,
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

	size_t decoded_size = 0;
	(void)utf16_decoded_length(str_len, str, &decoded_size);

	if (decoded_size > out_len) {
		return ENCODING_BUFFER_TOO_SMALL;
	}

	uint16_t* ptr = (uint16_t*)str;
	for (size_t i = 0; i < decoded_size; i += 1) {
		size_t tmp = 0;
		out[i]     = utf16_codepoint_decode(str_len - i, ptr, &tmp);
		ptr        = &ptr[tmp];
	}

	return 0;
}
