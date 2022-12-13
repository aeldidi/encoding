// SPDX-License-Identifier: 0BSD
// Copyright (C) 2022 Ayman El Didi
#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>

#include "../../include/encoding/binary.c"
#include "compiler_extensions.h"

// The ASCII '=' character is used for padding.
#define BASE64_PAD 0x3d

#define EXTERN

#if defined(__cplusplus)
#undef EXTERN
#define EXTERN extern
extern "C" {
#endif

// A lookup table for each 6-bit binary value to the corresponding base64
// byte.
ENCODING_PUBLIC
EXTERN
const uint8_t base64[64] = {
		// ASCII 'A' to 'Z'
		0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a,
		0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50, 0x51, 0x52, 0x53, 0x54,
		0x55, 0x56, 0x57, 0x58, 0x59, 0x5a,
		// ASCII 'a' to 'z'
		0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a,
		0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70, 0x71, 0x72, 0x73, 0x74,
		0x75, 0x76, 0x77, 0x78, 0x79, 0x7a,
		// ASCII '0' to '9'
		0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
		0x2b, // '+'
		0x2f, // '/'
};

// Map every possible byte to its base64 decoded counterpart. Before indexing,
// subtract the byte by 0x2b ('+'). This is done so that the valid 80 byte
// range appears at the beginning of the array, so they can be put into cache
// first. Any invalid bytes will be in the higher ranges of bytes, which will
// decode to 0x00.
//
// The same technique is applied for the base64url decoding table.
//
// Note: Since underflow is well defined on unsigned types, this doesn't invoke
//       any undefined behaviour.
static const uint8_t base64_decode_table[256] = {
		62, // '+'
		0, 0, 0,
		63, // '/'

		52, 53, 54, 55, 56, 57, 58, 59, 60, 61, // ASCII '0' to '9'
		0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
		13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
		25, // ASCII 'A' to 'Z'
		0, 0, 0, 0, 0, 0, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36,
		37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50,
		51, // ASCII 'a' to 'z'
};

// A lookup table for each 6-bit binary value to the corresponding base64url
// byte.
ENCODING_PUBLIC
EXTERN
const uint8_t base64url[64] = {
		// ASCII 'A' to 'Z'
		0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a,
		0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50, 0x51, 0x52, 0x53, 0x54,
		0x55, 0x56, 0x57, 0x58, 0x59, 0x5a,
		// ASCII 'a' to 'z'
		0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a,
		0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70, 0x71, 0x72, 0x73, 0x74,
		0x75, 0x76, 0x77, 0x78, 0x79, 0x7a,
		// ASCII '0' to '9'
		0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
		0x2d, // '-'
		0x5f, // '_'
};

// Same as the base64 table, but we shift by 0x2d ('-').
static const uint8_t base64url_decode_table[256] = {
		62, // '-'
		0, 0, 52, 53, 54, 55, 56, 57, 58, 59, 60,
		61, // ASCII '0' to '9'
		0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
		13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
		25,             // ASCII 'A' to 'Z'
		0, 0, 0, 0, 63, // '_'
		0, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
		41, 42, 43, 44, 45, 46, 47, 48, 49, 50,
		51, // ASCII 'a' to 'z'
};

ENCODING_PUBLIC
size_t
base64_encoded_length(const size_t len)
{
	return (len + 2) / 3 * 4;
}

static size_t
base64_encoded_unpadded_length(const size_t len)
{
	return (4 * len + 2) / 3;
}

static size_t
get_num_padding_chars(const size_t str_len, const uint8_t* str)
{
	size_t result = 0;
	for (size_t i = str_len; i != 0; i -= 1) {
		if (str[i - 1] != BASE64_PAD) {
			return result;
		}

		result += 1;
	}

	return result;
}

static size_t
base64_decoded_length_impl(const size_t str_len, const uint8_t* str)
{
	size_t num_padding_chars = get_num_padding_chars(str_len, str);
	if (num_padding_chars == str_len) {
		return 0;
	}

	return (3 * (str_len / 4)) - num_padding_chars;
}

static bool
base64_valid_fast(const size_t str_len, const uint8_t* str,
		const size_t num_padding_chars)
{
	bool result = true;
	// Since base64 chunks are 4 characters, we can check 4 chars at a
	// time.
	for (size_t i = 0; i + 4 < str_len - num_padding_chars; i += 4) {
		bool tmp1 = false;
		bool tmp2 = false;
		bool tmp3 = false;
		bool tmp4 = false;
		// 'A' to 'Z'
		tmp1 |= (str[i] >= 0x41) & (str[i] <= 0x5a);
		tmp2 |= (str[i + 1] >= 0x41) & (str[i + 1] <= 0x5a);
		tmp3 |= (str[i + 2] >= 0x41) & (str[i + 2] <= 0x5a);
		tmp4 |= (str[i + 3] >= 0x41) & (str[i + 3] <= 0x5a);
		// 'a' to 'z'
		tmp1 |= (str[i] >= 0x61) & (str[i] <= 0x7a);
		tmp2 |= (str[i + 1] >= 0x61) & (str[i + 1] <= 0x7a);
		tmp3 |= (str[i + 2] >= 0x61) & (str[i + 2] <= 0x7a);
		tmp4 |= (str[i + 3] >= 0x61) & (str[i + 3] <= 0x7a);
		// '0' to '9'
		tmp1 |= (str[i] >= 0x30) & (str[i] <= 0x39);
		tmp2 |= (str[i + 1] >= 0x30) & (str[i + 1] <= 0x39);
		tmp3 |= (str[i + 2] >= 0x30) & (str[i + 2] <= 0x39);
		tmp4 |= (str[i + 3] >= 0x30) & (str[i + 3] <= 0x39);
		// '+'
		tmp1 |= str[i] == 0x2b;
		tmp2 |= str[i + 1] == 0x2b;
		tmp3 |= str[i + 2] == 0x2b;
		tmp4 |= str[i + 3] == 0x2b;
		// '/'
		tmp1 |= str[i] == 0x2f;
		tmp2 |= str[i + 1] == 0x2f;
		tmp3 |= str[i + 2] == 0x2f;
		tmp4 |= str[i + 3] == 0x2f;

		result &= tmp1 & tmp2 & tmp3 & tmp4;
	}

	bool tmp1 = true;
	bool tmp3 = true;
	bool tmp2 = true;
	switch (num_padding_chars) {
	case 1:
		tmp1 = 0;
		tmp1 |= (str[str_len - 2] >= 0x41) &
			(str[str_len - 2] <= 0x5a);
		tmp1 |= (str[str_len - 2] >= 0x61) &
			(str[str_len - 2] <= 0x7a);
		tmp1 |= (str[str_len - 2] >= 0x30) &
			(str[str_len - 2] <= 0x39);
		tmp1 |= (str[str_len - 2] == 0x2b);
		tmp1 |= (str[str_len - 2] == 0x2f);
		// fallthrough
	case 2:
		tmp2 = 0;
		tmp2 |= (str[str_len - 4] >= 0x41) &
			(str[str_len - 4] <= 0x5a);
		tmp2 |= (str[str_len - 4] >= 0x61) &
			(str[str_len - 4] <= 0x7a);
		tmp2 |= (str[str_len - 4] >= 0x30) &
			(str[str_len - 4] <= 0x39);
		tmp2 |= str[str_len - 4] == 0x2b;
		tmp2 |= str[str_len - 4] == 0x2f;

		tmp3 = 0;
		tmp3 |= (str[str_len - 3] >= 0x41) &
			(str[str_len - 3] <= 0x5a);
		tmp3 |= (str[str_len - 3] >= 0x61) &
			(str[str_len - 3] <= 0x7a);
		tmp3 |= (str[str_len - 3] >= 0x30) &
			(str[str_len - 3] <= 0x39);
		tmp3 |= str[str_len - 3] == 0x2b;
		tmp3 |= str[str_len - 3] == 0x2f;
		// fallthrough
	default:
		break;
	}

	return result & tmp1 & tmp2 & tmp3;
}

static bool
base64url_valid_fast(const size_t str_len, const uint8_t* str,
		const size_t num_padding_chars)
{
	bool result = true;
	for (size_t i = 0; i + 4 < str_len - num_padding_chars; i += 4) {
		bool tmp1 = false;
		bool tmp2 = false;
		bool tmp3 = false;
		bool tmp4 = false;
		// 'A' to 'Z'
		tmp1 |= (str[i] >= 0x41) & (str[i] <= 0x5a);
		tmp2 |= (str[i + 1] >= 0x41) & (str[i + 1] <= 0x5a);
		tmp3 |= (str[i + 2] >= 0x41) & (str[i + 2] <= 0x5a);
		tmp4 |= (str[i + 3] >= 0x41) & (str[i + 3] <= 0x5a);
		// 'a' to 'z'
		tmp1 |= (str[i] >= 0x61) & (str[i] <= 0x7a);
		tmp2 |= (str[i + 1] >= 0x61) & (str[i + 1] <= 0x7a);
		tmp3 |= (str[i + 2] >= 0x61) & (str[i + 2] <= 0x7a);
		tmp4 |= (str[i + 3] >= 0x61) & (str[i + 3] <= 0x7a);
		// '0' to '9'
		tmp1 |= (str[i] >= 0x30) & (str[i] <= 0x39);
		tmp2 |= (str[i + 1] >= 0x30) & (str[i + 1] <= 0x39);
		tmp3 |= (str[i + 2] >= 0x30) & (str[i + 2] <= 0x39);
		tmp4 |= (str[i + 3] >= 0x30) & (str[i + 3] <= 0x39);
		// '-'
		tmp1 |= str[i] == 0x2d;
		tmp2 |= str[i + 1] == 0x2d;
		tmp3 |= str[i + 2] == 0x2d;
		tmp4 |= str[i + 3] == 0x2d;
		// '_'
		tmp1 |= str[i] == 0x5f;
		tmp2 |= str[i + 1] == 0x5f;
		tmp3 |= str[i + 2] == 0x5f;
		tmp4 |= str[i + 3] == 0x5f;

		result &= tmp1 & tmp2 & tmp3 & tmp4;
	}

	bool tmp1 = true;
	bool tmp2 = true;
	bool tmp3 = true;
	switch (num_padding_chars) {
	case 1:
		tmp1 = 0;
		tmp1 |= (str[str_len - 2] >= 0x41) &
			(str[str_len - 2] <= 0x5a);
		tmp1 |= (str[str_len - 2] >= 0x61) &
			(str[str_len - 2] <= 0x7a);
		tmp1 |= (str[str_len - 2] >= 0x30) &
			(str[str_len - 2] <= 0x39);
		tmp1 |= (str[str_len - 2] == 0x2d);
		tmp1 |= (str[str_len - 2] == 0x5f);
		// fallthrough
	case 2:
		tmp2 = 0;
		tmp2 |= (str[str_len - 3] >= 0x41) &
			(str[str_len - 3] <= 0x5a);
		tmp2 |= (str[str_len - 3] >= 0x61) &
			(str[str_len - 3] <= 0x7a);
		tmp2 |= (str[str_len - 3] >= 0x30) &
			(str[str_len - 3] <= 0x39);
		tmp2 |= (str[str_len - 3] == 0x2d);
		tmp2 |= (str[str_len - 3] == 0x5f);

		tmp3 = 0;
		tmp3 |= (str[str_len - 4] >= 0x41) &
			(str[str_len - 4] <= 0x5a);
		tmp3 |= (str[str_len - 4] >= 0x61) &
			(str[str_len - 4] <= 0x7a);
		tmp3 |= (str[str_len - 4] >= 0x30) &
			(str[str_len - 4] <= 0x39);
		tmp3 |= (str[str_len - 4] == 0x2d);
		tmp3 |= (str[str_len - 4] == 0x5f);
		// fallthrough
	default:
		break;
	}

	return result & tmp1 & tmp2 & tmp3;
}

ENCODING_PUBLIC
bool
base64_valid(const size_t str_len, const uint8_t* str,
		const uint8_t alphabet[64])
{
	if (UNLIKELY(str_len == 0)) {
		return true;
	}

	if (UNLIKELY(alphabet == NULL || str == NULL)) {
		return false;
	}

	size_t num_padding_chars = get_num_padding_chars(str_len, str);
	if (UNLIKELY(num_padding_chars > 2 || str_len % 4 != 0)) {
		return false;
	}

	if (alphabet == base64) {
		return base64_valid_fast(str_len, str, num_padding_chars);
	}

	if (alphabet == base64url) {
		return base64url_valid_fast(str_len, str, num_padding_chars);
	}

	bool result = true;
	for (size_t i = 0; i + 4 < str_len - num_padding_chars; i += 4) {
		int valid1 = 0;
		int valid2 = 0;
		int valid3 = 0;
		int valid4 = 0;
		for (size_t j = 0; j < 64; j += 1) {
			valid1 |= alphabet[j] == str[i];
			valid2 |= alphabet[j] == str[i + 1];
			valid3 |= alphabet[j] == str[i + 2];
			valid4 |= alphabet[j] == str[i + 3];
		}

		result &= valid1 & valid2 & valid3 & valid4;
	}

	int tmp1 = 1;
	int tmp2 = 1;
	int tmp3 = 1;
	switch (num_padding_chars) {
	case 1:
		tmp1 = 0;
		for (size_t i = 0; i < 64; i += 1) {
			tmp1 |= alphabet[i] == str[str_len - 2];
		}

		// fallthrough
	case 2:
		tmp2 = 0;
		tmp3 = 0;
		for (size_t i = 0; i < 64; i += 1) {
			tmp2 |= alphabet[i] == str[str_len - 3];
			tmp3 |= alphabet[i] == str[str_len - 4];
		}

		// fallthrough
	default:
		break;
	}

	return result & tmp1 & tmp2 & tmp3;
}

ENCODING_PUBLIC
int
base64_encode(const size_t str_len, const uint8_t* str, const size_t out_len,
		uint8_t* out, const uint8_t alphabet[64])
{
	assert(alphabet != NULL);

	if (UNLIKELY(str_len == 0)) {
		return 0;
	}

	assert(str != NULL);

	size_t encoded_len = base64_encoded_length(str_len);
	if (UNLIKELY(out == NULL || out_len < encoded_len)) {
		return ENCODING_BUFFER_TOO_SMALL;
	}

	size_t j = 0;
	size_t i = 0;
	while (i + 2 < str_len) {
		out[j]     = alphabet[str[i] >> 2];
		out[j + 1] = alphabet[(str[i] & 0x3) << 4 | str[i + 1] >> 4];
		out[j + 2] = alphabet[(str[i + 1] & 0xf) << 2 |
				      str[i + 2] >> 6];
		out[j + 3] = alphabet[str[i + 2] & 0x3f];

		j += 4;
		i += 3;
	}

	size_t unpadded_len = base64_encoded_unpadded_length(str_len);
	switch (encoded_len - unpadded_len) {
	case 2:
		out[j]     = alphabet[str[i] >> 2];
		out[j + 1] = alphabet[(str[i] & 0x3) << 4];
		out[j + 2] = BASE64_PAD;
		out[j + 3] = BASE64_PAD;
		break;
	case 1:
		out[j]     = alphabet[str[i] >> 2];
		out[j + 1] = alphabet[(str[i] & 0x3) << 4 | str[i + 1] >> 4];
		out[j + 2] = alphabet[(str[i + 1] & 0xf) << 2];
		out[j + 3] = BASE64_PAD;
		break;
	default:
		break;
	}

	return 0;
}

ENCODING_PUBLIC
size_t
base64_decoded_length(const size_t str_len, const uint8_t* str)
{
	if (UNLIKELY(str_len == 0)) {
		return 0;
	}

	assert(str != NULL);

	return base64_decoded_length_impl(str_len, str);
}

static uint8_t
alphabet_find(const uint8_t ch, const uint8_t alphabet[64])
{
	for (uint8_t i = 0; i < 64; i += 1) {
		if (ch == alphabet[i]) {
			return i;
		}
	}

	return UINT8_MAX;
}

static int
decode_fast(const size_t str_len, const uint8_t* str, uint8_t* out,
		const uint8_t decode_table[256], const uint8_t shift_amount)
{
	size_t j = 0;
	for (size_t i = 0; i < str_len; i += 4, j += 3) {
		uint8_t byte1 = str[i] - shift_amount;
		uint8_t byte2 = str[i + 1] - shift_amount;
		uint8_t byte3 = str[i + 2] - shift_amount;
		uint8_t byte4 = str[i + 3] - shift_amount;

		out[j] = (decode_table[byte1] & 0x3f) << 2 |
			 (decode_table[byte2] & 0x30) >> 4;
		out[j + 1] = (decode_table[byte2] & 0x0f) << 4 |
			     (decode_table[byte3] & 0x3c) >> 2;
		out[j + 2] = (decode_table[byte3] & 0x03) << 6 |
			     (decode_table[byte4] & 0x3f);
	}

	return 0;
}

ENCODING_PUBLIC
int
base64_decode(const size_t str_len, const uint8_t* str, const size_t out_len,
		uint8_t* out, const uint8_t alphabet[64])
{
	assert(alphabet != NULL);

	if (UNLIKELY(str_len == 0)) {
		return 0;
	}

	assert(str != NULL);

	if (UNLIKELY(str_len % 4 != 0)) {
		return ENCODING_INVALID_ARGUMENT;
	}

	size_t decoded_len = base64_decoded_length(str_len, str);
	if (UNLIKELY(out_len < decoded_len)) {
		return ENCODING_BUFFER_TOO_SMALL;
	}

	assert(out_len == 0 || out != NULL);

	if (alphabet == base64) {
		return decode_fast(
				str_len, str, out, base64_decode_table, 0x2b);
	}

	if (alphabet == base64url) {
		return decode_fast(str_len, str, out, base64url_decode_table,
				0x2d);
	}

	size_t out_index = 0;
	for (size_t i = 0; i < str_len; i += 4, out_index += 3) {
		uint8_t byte1 = alphabet_find(str[i + 0], alphabet);
		uint8_t byte2 = alphabet_find(str[i + 1], alphabet);
		uint8_t byte3 = alphabet_find(str[i + 2], alphabet);
		uint8_t byte4 = alphabet_find(str[i + 3], alphabet);

		out[out_index]     = byte1 << 2 | byte2 >> 4;
		out[out_index + 1] = byte2 << 4 | byte3 >> 2;
		out[out_index + 2] = ((byte3 << 6) & 0xc0) | byte4;
	}

	return 0;
}

#if defined(__cplusplus)
}
#endif
#undef EXTERN
