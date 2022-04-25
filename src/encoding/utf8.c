// SPDX-License-Identifier: BSL-1.0
// Copyright (C) 2022 Ayman El Didi
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "encoding/utf8.h"

// The Unicode replacement character (U+FFFD) is sometimes used when invalid
// characters are encountered.
static const uint8_t utf8_replacement[] = {0xef, 0xbf, 0xbd};

#define UTF8_ACCEPT (0)
#define UTF8_REJECT (-1)

// https://nullprogram.com/blog/2020/12/31/
static const int utf8_table[8][256] = {
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
				1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
				1, 1, +3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
				4, 2, 2, 5, 6, 6, 6, 7, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1},
		{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
				1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
				1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
				1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
				1, 1, 1, 1, 1, 1, 1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
				1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
				1, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1},
		{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
				1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
				1, 1, 1, 1, 1, 1, 1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1},
		{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, 2, 2, 2, 2,
				2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
				2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
				2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1},
		{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, 2, 2, 2, 2, 2, 2, 2, 2, 2,
				2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
				2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
				2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
				2, 2, 2, 2, 2, 2, 2, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, 2, 2, 2, 2, 2, 2, 2, 2, 2,
				2, 2, 2, 2, 2, 2, 2, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1}};
static unsigned char masks[2][8] = {
		{0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f},
		{0x7f, 0x1f, 0x0f, 0x0f, 0x0f, 0x07, 0x07, 0x07}};

static int
utf8_state_advance(const int state, uint32_t* codep, const uint8_t byte)
{
	int next = utf8_table[state][byte];
	*codep   = (*codep << 6) | ((int)byte & masks[!state][next & 7]);
	return next;
}

bool
utf8_valid(const size_t str_len, const uint8_t* str)
{
	if (str == NULL) {
		return false;
	}

	uint32_t codepoint = 0;
	int      state     = UTF8_ACCEPT;
	for (size_t i = 0; i < str_len; i += 1) {
		if (state == UTF8_REJECT) {
			return false;
		}
		state = utf8_state_advance(state, &codepoint, str[i]);
	}

	if (state != UTF8_ACCEPT) {
		return false;
	}

	return true;
}

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

	uint8_t* bytes = (uint8_t*)out;

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

	int      state     = 0;
	uint32_t codepoint = 0;
	size_t   result    = 0;
	for (size_t i = 0; i < str_len; i += 1) {
		state = utf8_state_advance(state, &codepoint, str[i]);
		switch (state) {
		case UTF8_REJECT:
			state = 0;
			// fallthrough
		case UTF8_ACCEPT:
			result += 1;
			break;
		default:
			break;
		}
	}

	*out = result;
	return 0;
}

uint32_t
utf8_codepoint_decode(const size_t str_len, const uint8_t* str, size_t* size)
{
	if (size != NULL) {
		*size = 3;
	}

	if (str == NULL || str_len == 0) {
		return ENCODING_CODEPOINT_ERROR;
	}

	uint32_t result    = 0;
	int      state     = 0;
	size_t   next_size = 0;
	for (size_t i = 0; i < str_len; i += 1) {
		state = utf8_state_advance(state, &result, str[i]);
		next_size += 1;
		if (state == UTF8_REJECT || state == UTF8_ACCEPT) {
			break;
		}
	}

	if (state != UTF8_ACCEPT) {
		return ENCODING_CODEPOINT_ERROR;
	}

	if (size != NULL) {
		*size = next_size;
	}

	return result;
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

	uint32_t result = 0;
	int      state  = 0;
	size_t   j      = 0;
	size_t   i      = 0;
	while (i < str_len && j < out_len) {
		state = utf8_state_advance(state, &result, str[i]);
		switch (state) {
		case UTF8_ACCEPT:
			out[j] = result;
			result = 0;
			j += 1;
			break;
		case UTF8_REJECT:
			out[j] = ENCODING_CODEPOINT_ERROR;
			result = 0;
			state  = 0;
			j += 1;
			break;
		default:
			break;
		}

		i += 1;
	}

	if (i < str_len || state != 0) {
		// We don't want to partially write to out if we couldn't
		// decode the entire buffer.
		for (size_t k = 0; k < out_len; k += 1) {
			out[k] = 0;
		}

		// A codepoint in the input was truncated.
		if (state != 0) {
			return ENCODING_INVALID_ARGUMENT;
		}

		return ENCODING_BUFFER_TOO_SMALL;
	}

	return 0;
}
