// SPDX-License-Identifier: 0BSD
// Copyright (C) 2022 Ayman El Didi
#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>

#include "../binary/binary.c"
#include "../compiler_extensions.h"
#include "./base32.h"

#define EXTERN

#if defined(__cplusplus)
#undef EXTERN
#define EXTERN extern
extern "C" {
#endif

// The ASCII '=' character is used for padding.
#define BASE32_PAD 0x3d

ENCODING_PUBLIC
EXTERN
const uint8_t base32[32] = {
		// ASCII 'A' to 'Z'
		0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a,
		0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50, 0x51, 0x52, 0x53, 0x54,
		0x55, 0x56, 0x57, 0x58, 0x59, 0x5a,
		// ASCII '2' to '7'
		0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
		//
};

ENCODING_PUBLIC
EXTERN
const uint8_t base32hex[32] = {
		// ASCII '0' to '9'
		0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
		// ASCII 'A' to 'V'
		0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a,
		0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50, 0x51, 0x52, 0x53, 0x54,
		0x55, 0x56,
		//
};

#define ASCII_I 0x49
#define ASCII_L 0x4c
#define ASCII_O 0x4f
#define ASCII_U 0x55

#define ASCII_i 0x69
#define ASCII_l 0x6c
#define ASCII_o 0x6f
#define ASCII_u 0x75

ENCODING_PUBLIC
EXTERN
const uint8_t base32crockford[32] = {
		// ASCII '0' to '9'
		0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
		// ASCII 'A' to 'Z', excluding 'I', 'L', 'O', 'U'
		0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x4a, 0x4b,
		0x4d, 0x4e, 0x50, 0x51, 0x52, 0x53, 0x54, 0x56, 0x57, 0x58,
		0x59, 0x5a,
		//
};

static size_t
get_num_padding_chars(const size_t str_len, const uint8_t* str)
{
	size_t result = 0;
	for (size_t i = str_len; i != 0; i -= 1) {
		if (str[i - 1] != BASE32_PAD) {
			return result;
		}

		result += 1;
	}

	return result;
}

static bool
base32_isalpha(uint8_t c)
{
	return ((c | 32) - 0x61) < 26;
}

static bool
base32_isdigit(uint8_t c)
{
	return c - 0x30 < 10;
}

static bool
base32_valid_fast(const size_t str_len, const uint8_t* str,
		const size_t num_padding_chars)
{
	int result = 1;
	for (size_t i = 0; i < str_len - num_padding_chars; i += 8) {
		// 'A' to 'Z'
		int tmp1 = (str[i] - 0x41) <= 0x19;
		int tmp2 = (str[i + 1] - 0x41) <= 0x19;
		int tmp3 = (str[i + 2] - 0x41) <= 0x19;
		int tmp4 = (str[i + 3] - 0x41) <= 0x19;
		int tmp5 = (str[i + 4] - 0x41) <= 0x19;
		int tmp6 = (str[i + 5] - 0x41) <= 0x19;
		int tmp7 = (str[i + 6] - 0x41) <= 0x19;
		int tmp8 = (str[i + 7] - 0x41) <= 0x19;

		// '2' to '7'
		tmp1 |= (str[i] - 0x32) <= 5;
		tmp2 |= (str[i + 1] - 0x32) <= 5;
		tmp3 |= (str[i + 2] - 0x32) <= 5;
		tmp4 |= (str[i + 3] - 0x32) <= 5;
		tmp5 |= (str[i + 4] - 0x32) <= 5;
		tmp6 |= (str[i + 5] - 0x32) <= 5;
		tmp7 |= (str[i + 6] - 0x32) <= 5;
		tmp8 |= (str[i + 7] - 0x32) <= 5;

		result &= tmp1 & tmp2 & tmp3 & tmp4 & tmp5 & tmp6 & tmp7 &
			  tmp8;
	}

	return result;
}

static bool
base32hex_valid_fast(const size_t str_len, const uint8_t* str,
		const size_t num_padding_chars)
{
	int result = 1;
	for (size_t i = 0; i < str_len - num_padding_chars; i += 8) {
		// '0' to '9'
		int tmp1 = base32_isdigit(str[i]);
		int tmp2 = base32_isdigit(str[i + 1]);
		int tmp3 = base32_isdigit(str[i + 2]);
		int tmp4 = base32_isdigit(str[i + 3]);
		int tmp5 = base32_isdigit(str[i + 4]);
		int tmp6 = base32_isdigit(str[i + 5]);
		int tmp7 = base32_isdigit(str[i + 6]);
		int tmp8 = base32_isdigit(str[i + 7]);

		// 'A' to 'V'
		tmp1 |= (str[i] - 0x41) <= 0x15;
		tmp2 |= (str[i + 1] - 0x41) <= 0x15;
		tmp3 |= (str[i + 2] - 0x41) <= 0x15;
		tmp4 |= (str[i + 3] - 0x41) <= 0x15;
		tmp5 |= (str[i + 4] - 0x41) <= 0x15;
		tmp6 |= (str[i + 5] - 0x41) <= 0x15;
		tmp7 |= (str[i + 6] - 0x41) <= 0x15;
		tmp8 |= (str[i + 7] - 0x41) <= 0x15;

		result &= tmp1 & tmp2 & tmp3 & tmp4 & tmp5 & tmp6 & tmp7 &
			  tmp8;
	}

	return result;
}

static bool
base32crockford_valid(const size_t str_len, const uint8_t* str,
		const size_t num_padding_chars)
{
	int result = 1;
	for (size_t i = 0; i < str_len - num_padding_chars; i += 8) {
		// 'A' to 'Z' and 'a' to 'z'
		int tmp1 = base32_isalpha(str[i]);
		int tmp2 = base32_isalpha(str[i + 1]);
		int tmp3 = base32_isalpha(str[i + 2]);
		int tmp4 = base32_isalpha(str[i + 3]);
		int tmp5 = base32_isalpha(str[i + 4]);
		int tmp6 = base32_isalpha(str[i + 5]);
		int tmp7 = base32_isalpha(str[i + 6]);
		int tmp8 = base32_isalpha(str[i + 7]);

		// '0' to '9'
		tmp1 |= base32_isdigit(str[i]);
		tmp2 |= base32_isdigit(str[i + 1]);
		tmp3 |= base32_isdigit(str[i + 2]);
		tmp4 |= base32_isdigit(str[i + 3]);
		tmp5 |= base32_isdigit(str[i + 4]);
		tmp6 |= base32_isdigit(str[i + 5]);
		tmp7 |= base32_isdigit(str[i + 6]);
		tmp8 |= base32_isdigit(str[i + 7]);

		result &= tmp1 & tmp2 & tmp3 & tmp4 & tmp5 & tmp6 & tmp7 &
			  tmp8;
	}

	return result;
}

// base32_valid returns true if str is valid base32, false otherwise.
ENCODING_PUBLIC
bool
base32_valid(const size_t str_len, const uint8_t* str,
		const uint8_t alphabet[32])
{
	if (UNLIKELY(str_len == 0)) {
		return true;
	}

	if (UNLIKELY(alphabet == NULL || str == NULL)) {
		return false;
	}

	size_t num_padding_chars = get_num_padding_chars(str_len, str);
	if (UNLIKELY(num_padding_chars > 6 || str_len % 8 != 0)) {
		return false;
	}

	if (alphabet == base32) {
		return base32_valid_fast(str_len, str, num_padding_chars);
	}

	if (alphabet == base32hex) {
		return base32hex_valid_fast(str_len, str, num_padding_chars);
	}

	if (alphabet == base32crockford) {
		return base32crockford_valid(str_len, str, num_padding_chars);
	}

	int    result = 1;
	size_t i      = 0;
	for (; i + 8 < str_len - num_padding_chars; i += 8) {
		int valid1 = 0;
		int valid2 = 0;
		int valid3 = 0;
		int valid4 = 0;
		int valid5 = 0;
		int valid6 = 0;
		int valid7 = 0;
		int valid8 = 0;

		for (size_t j = 0; j < 32; j += 1) {
			valid1 |= str[i] == alphabet[j];
			valid2 |= str[i + 1] == alphabet[j];
			valid3 |= str[i + 2] == alphabet[j];
			valid4 |= str[i + 3] == alphabet[j];
			valid5 |= str[i + 4] == alphabet[j];
			valid6 |= str[i + 5] == alphabet[j];
			valid7 |= str[i + 6] == alphabet[j];
			valid8 |= str[i + 7] == alphabet[j];
		}

		result &= valid1 & valid2 & valid3 & valid4 & valid5 & valid6 &
			  valid7 & valid8;
	}

	i -= 8;

	for (; i < str_len - num_padding_chars; i += 1) {
		int tmp = 0;
		for (size_t j = 0; j < 32; j += 1) {
			tmp |= str[i] == alphabet[j];
		}

		result &= tmp;
	}

	return result;
}

// base32_encoded_length returns the base32 encoded size of the
// base32-encoded data with the length str_len.
ENCODING_PUBLIC
size_t
base32_encoded_length(const size_t str_len)
{
	const size_t result = str_len * 8 / 5;
	return ((result + 8 - 1) / 8) * 8;
}

static size_t
base32_decoded_length_impl(const size_t str_len, const uint8_t* str)
{
	size_t num_padding_chars = get_num_padding_chars(str_len, str);
	if (num_padding_chars == str_len) {
		return 0;
	}

	return (str_len - num_padding_chars) * 5 / 8;
}

ENCODING_PUBLIC
size_t
base32_decoded_length(const size_t str_len, const uint8_t* str)
{
	if (UNLIKELY(str_len == 0)) {
		return 0;
	}

	assert(str != NULL);

	return base32_decoded_length_impl(str_len, str);
}

ENCODING_PUBLIC
int
base32_encode(const size_t str_len, const uint8_t* str, const size_t out_len,
		uint8_t* out, const uint8_t alphabet[32])
{
	assert(alphabet != NULL);

	if (UNLIKELY(str_len == 0)) {
		return 0;
	}

	assert(str != NULL);

	size_t encoded_len = base32_encoded_length(str_len);
	if (UNLIKELY(out == NULL || out_len < encoded_len)) {
		return ENCODING_BUFFER_TOO_SMALL;
	}

	size_t j = 0;
	size_t i = 0;
	while (i + 4 < str_len) {
		out[j]     = alphabet[(str[i] >> 3)];
		out[j + 1] = alphabet[(str[i] & 0x07) << 2 | str[i + 1] >> 6];
		out[j + 2] = alphabet[(str[i + 1] & 0x3e) >> 1];
		out[j + 3] = alphabet[(str[i + 1] & 0x01) << 4 |
				      (str[i + 2] & 0xf0) >> 4];
		out[j + 4] = alphabet[(str[i + 2] & 0x0f) << 1 |
				      (str[i + 3] & 0x80) >> 7];
		out[j + 5] = alphabet[(str[i + 3] & 0x7f) >> 2];
		out[j + 6] = alphabet[(str[i + 3] & 0x03) << 3 |
				      (str[i + 4] >> 5)];
		out[j + 7] = alphabet[(str[i + 4] & 0x1f)];

		i += 5;
		j += 8;
	}

	// How many characters are left?
	switch (str_len - i) {
	case 4:
		out[j]     = alphabet[(str[i] >> 3)];
		out[j + 1] = alphabet[(str[i] & 0x07) << 2 | str[i + 1] >> 6];
		out[j + 2] = alphabet[(str[i + 1] & 0x3e) >> 1];
		out[j + 3] = alphabet[(str[i + 1] & 0x01) << 4 |
				      (str[i + 2] & 0xf0) >> 4];
		out[j + 4] = alphabet[(str[i + 2] & 0x0f) << 1 |
				      (str[i + 3] & 0x80) >> 7];
		out[j + 5] = alphabet[(str[i + 3] & 0x7f) >> 2];
		out[j + 6] = alphabet[(str[i + 3] & 0x03) << 3];
		out[j + 7] = BASE32_PAD;
		break;
	case 3:
		out[j]     = alphabet[(str[i] >> 3)];
		out[j + 1] = alphabet[(str[i] & 0x07) << 2 | str[i + 1] >> 6];
		out[j + 2] = alphabet[(str[i + 1] & 0x3e) >> 1];
		out[j + 3] = alphabet[(str[i + 1] & 0x01) << 4 |
				      (str[i + 2] & 0xf0) >> 4];
		out[j + 4] = alphabet[(str[i + 2] & 0x0f) << 1];
		out[j + 5] = BASE32_PAD;
		out[j + 6] = BASE32_PAD;
		out[j + 7] = BASE32_PAD;
		break;
	case 2:
		out[j]     = alphabet[(str[i] >> 3)];
		out[j + 1] = alphabet[(str[i] & 0x07) << 2 | str[i + 1] >> 6];
		out[j + 2] = alphabet[(str[i + 1] & 0x3e) >> 1];
		out[j + 3] = alphabet[(str[i + 1] & 0x01) << 4];
		out[j + 4] = BASE32_PAD;
		out[j + 5] = BASE32_PAD;
		out[j + 6] = BASE32_PAD;
		out[j + 7] = BASE32_PAD;
		break;
	case 1:
		out[j]     = alphabet[(str[i] >> 3)];
		out[j + 1] = alphabet[(str[i] & 0x07) << 2];
		out[j + 2] = BASE32_PAD;
		out[j + 3] = BASE32_PAD;
		out[j + 4] = BASE32_PAD;
		out[j + 5] = BASE32_PAD;
		out[j + 6] = BASE32_PAD;
		out[j + 7] = BASE32_PAD;
		break;
	default:
		break;
	}

	return 0;
}

static uint8_t
alphabet_find(const uint8_t ch, const uint8_t alphabet[32])
{
	for (uint8_t i = 0; i < 32; i += 1) {
		if (ch == alphabet[i]) {
			return i;
		}
	}

	return UINT8_MAX;
}

static uint8_t
crockford_normalize(uint8_t c)
{
	if (base32_isdigit(c)) {
		return c;
	}

	c &= 0x5f;

	switch (c) {
	case ASCII_I: // fallthrough
	case ASCII_L:
		return 0x31;
	case ASCII_O:
		return 0x30;
	default:
		return c;
	}
}

static int
base32crockford_decode(const size_t str_len, const uint8_t* str, uint8_t* out)
{
	size_t out_index = 0;
	for (size_t i = 0; i < str_len; i += 8, out_index += 5) {
		uint8_t byte1 = alphabet_find(crockford_normalize(str[i + 0]),
				base32crockford);
		uint8_t byte2 = alphabet_find(crockford_normalize(str[i + 1]),
				base32crockford);
		uint8_t byte3 = alphabet_find(crockford_normalize(str[i + 2]),
				base32crockford);
		uint8_t byte4 = alphabet_find(crockford_normalize(str[i + 3]),
				base32crockford);
		uint8_t byte5 = alphabet_find(crockford_normalize(str[i + 4]),
				base32crockford);
		uint8_t byte6 = alphabet_find(crockford_normalize(str[i + 5]),
				base32crockford);
		uint8_t byte7 = alphabet_find(crockford_normalize(str[i + 6]),
				base32crockford);
		uint8_t byte8 = alphabet_find(crockford_normalize(str[i + 7]),
				base32crockford);

		out[out_index]     = (byte1 << 3) | (byte2 >> 2);
		out[out_index + 1] = ((byte2 & 0x03) << 6) | (byte3 << 1) |
				     (byte4 >> 4);
		out[out_index + 2] = (byte4 << 4) | (byte5 >> 1);
		out[out_index + 3] =
				(byte5 << 7) | (byte6 << 2) | (byte7 >> 3);
		out[out_index + 4] = (byte7 << 5) | byte8;
	}

	return 0;
}

ENCODING_PUBLIC
int
base32_decode(const size_t str_len, const uint8_t* str, const size_t out_len,
		uint8_t* out, const uint8_t alphabet[32])
{
	assert(alphabet != NULL);

	if (UNLIKELY(str_len == 0)) {
		return 0;
	}

	assert(str != NULL);

	if (UNLIKELY(str_len % 8 != 0)) {
		return ENCODING_INVALID_ARGUMENT;
	}

	size_t decoded_len = base32_decoded_length(str_len, str);
	if (UNLIKELY(out_len < decoded_len)) {
		return ENCODING_BUFFER_TOO_SMALL;
	}

	assert(out_len == 0 || out != NULL);

	if (alphabet == base32crockford) {
		return base32crockford_decode(str_len, str, out);
	}

	size_t out_index = 0;
	for (size_t i = 0; i < str_len; i += 8, out_index += 5) {
		uint8_t byte1 = alphabet_find(str[i + 0], alphabet);
		uint8_t byte2 = alphabet_find(str[i + 1], alphabet);
		uint8_t byte3 = alphabet_find(str[i + 2], alphabet);
		uint8_t byte4 = alphabet_find(str[i + 3], alphabet);
		uint8_t byte5 = alphabet_find(str[i + 4], alphabet);
		uint8_t byte6 = alphabet_find(str[i + 5], alphabet);
		uint8_t byte7 = alphabet_find(str[i + 6], alphabet);
		uint8_t byte8 = alphabet_find(str[i + 7], alphabet);

		out[out_index]     = (byte1 << 3) | (byte2 >> 2);
		out[out_index + 1] = ((byte2 & 0x03) << 6) | (byte3 << 1) |
				     (byte4 >> 4);
		out[out_index + 2] = (byte4 << 4) | (byte5 >> 1);
		out[out_index + 3] =
				(byte5 << 7) | (byte6 << 2) | (byte7 >> 3);
		out[out_index + 4] = (byte7 << 5) | byte8;
	}

	return 0;
}

#if defined(__cplusplus)
}
#endif
#undef EXTERN
