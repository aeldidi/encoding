// SPDX-License-Identifier: 0BSD
// Copyright (C) 2022 Ayman El Didi
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "encoding/binary.h"
#include "encoding/hex.h"

const uint8_t hex_chars[] = {
		// '0' to '9'
		0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
		// 'a' to 'f'
		0x61, 0x62, 0x63, 0x64, 0x65, 0x66,
		// 'A' to 'F'
		0x41, 0x42, 0x43, 0x44, 0x45, 0x46,
		//
};

const uint8_t char_to_nybble[0x67] = {
		// '0' to '9'
		[0x30] = 0,
		[0x31] = 1,
		[0x32] = 2,
		[0x33] = 3,
		[0x34] = 4,
		[0x35] = 5,
		[0x36] = 6,
		[0x37] = 7,
		[0x38] = 8,
		[0x39] = 9,
		// 'a' to 'z'
		[0x61] = 10,
		[0x62] = 11,
		[0x63] = 12,
		[0x64] = 13,
		[0x65] = 14,
		[0x66] = 15,
		// 'A' to 'Z'
		[0x41] = 10,
		[0x42] = 11,
		[0x43] = 12,
		[0x44] = 13,
		[0x45] = 14,
		[0x46] = 15,
};

bool
hex_valid(const size_t str_len, const uint8_t* str)
{
	if (str == NULL || str_len % 2 != 0) {
		return false;
	}

	if (str_len == 0) {
		return true;
	}

	int result = true;
	for (size_t i = 0; i < str_len; i += 2) {
		int tmp1 = (str[i] >= 0x30) & (str[i] <= 0x39);
		int tmp2 = (str[i + 1] >= 0x30) & (str[i + 1] <= 0x39);

		tmp1 |= (str[i] >= 0x41) & (str[i] <= 0x46);
		tmp2 |= (str[i + 1] >= 0x41) & (str[i + 1] <= 0x46);

		tmp1 |= (str[i] >= 0x61) & (str[i] <= 0x66);
		tmp2 |= (str[i + 1] >= 0x61) & (str[i + 1] <= 0x66);

		result &= tmp1 & tmp2;
	}

	return result;
}

size_t
hex_encoded_length(const size_t len)
{
	return len * 2;
}

int
hex_encode(const size_t str_len, const uint8_t* str, const size_t out_len,
		uint8_t* out)
{
	if (str_len == 0) {
		return 0;
	}

	if (str == NULL) {
		return ENCODING_INVALID_NULL_POINTER;
	}

	if (out_len < hex_encoded_length(str_len)) {
		return ENCODING_BUFFER_TOO_SMALL;
	}

	if (out == NULL) {
		return ENCODING_INVALID_NULL_POINTER;
	}

	size_t out_index = 0;
	for (size_t i = 0; i < str_len; i += 1) {
		out[out_index]     = hex_chars[(str[i] & 0xf0) >> 4];
		out[out_index + 1] = hex_chars[(str[i] & 0x0f) >> 0];
		out_index += 2;
	}

	return 0;
}

size_t
hex_decoded_length(const size_t len)
{
	return len / 2;
}

int
hex_decode(const size_t str_len, const uint8_t* str, const size_t out_len,
		uint8_t* out)
{
	if (str_len > 0 && str == NULL) {
		return ENCODING_INVALID_NULL_POINTER;
	}

	if (str_len % 2 != 0) {
		return ENCODING_INVALID_ARGUMENT;
	}

	if (hex_decoded_length(str_len) > out_len) {
		return ENCODING_BUFFER_TOO_SMALL;
	}

	if (out == NULL) {
		return ENCODING_INVALID_NULL_POINTER;
	}

	size_t out_index = 0;
	for (size_t i = 0; i < str_len; i += 2) {
		out[out_index] = char_to_nybble[str[i]] << 4 |
				 char_to_nybble[str[i + 1]] << 0;
		out_index += 1;
	}

	return 0;
}

#define HEX_DUMP_LINE_LEN 78

size_t
hex_dump_length(const size_t len)
{
	size_t num_lines = (len / 16) + (len % 16);
	return num_lines * HEX_DUMP_LINE_LEN;
}

// A small buffer to hold the address in the hex dump output.
static uint8_t address_buffer[8] = {0};

// Fills address_buffer with num formatted as a string of 8 hexadecimal
// characters.
static void
uint64_to_str(uint64_t num)
{
	uint8_t tmp[8] = {0};
	size_t  sz     = 0;
	for (size_t i = 0; i < 8; i += 1) {
		if (num == 0) {
			break;
		}

		tmp[i] = hex_chars[num % 16];
		num /= 16;
		sz += 1;
	}

	size_t tmp_index = sz;
	for (size_t i = 0; i < 8; i += 1) {
		if (i < 8 - sz) {
			address_buffer[i] = hex_chars[0];
		} else {
			address_buffer[i] = tmp[tmp_index - 1];
			tmp_index -= 1;
		}
	}
}

#define UTF8_SPACE   0x20 // ' '
#define UTF8_PERIOD  0x2e // '.'
#define UTF8_NEWLINE 0x0a // '\n'
#define UTF8_PIPE    0x7c // '|'

int
hex_dump(const size_t str_len, const uint8_t* str, const size_t out_len,
		uint8_t* out, uint64_t* offset)
{
	if (str == NULL && str_len > 0) {
		return ENCODING_INVALID_NULL_POINTER;
	}

	if (hex_dump_length(str_len) > out_len) {
		return ENCODING_BUFFER_TOO_SMALL;
	}

	if (out == NULL) {
		return ENCODING_INVALID_NULL_POINTER;
	}

	size_t address = 0;
	if (offset != NULL) {
		address = *offset;
		// Detects if address value would wrap around.
		if (address > SIZE_MAX - str_len) {
			return ENCODING_INVALID_ARGUMENT;
		}
	}

	size_t out_index = 0;
	for (size_t i = 0; i < str_len; i += 16) {
		// Address column
		uint64_to_str(address);
		for (size_t j = 0; j < 8; j += 1) {
			out[out_index + j] = address_buffer[j];
		}

		out_index += 8;
		out[out_index]     = UTF8_SPACE;
		out[out_index + 1] = UTF8_SPACE;
		out_index += 2;

		address += 16;

		// 8 hex byte wide columns * 2
		for (size_t j = 0; j < 2; j += 1) {
			for (size_t k = 0; k < 8; k += 1) {
				const size_t idx = (i + k) + (j * 8);
				if (idx > str_len - 1) {
					out[out_index]     = UTF8_SPACE;
					out[out_index + 1] = UTF8_SPACE;
				} else {
					(void)hex_encode(1, &str[idx],
							SIZE_MAX,
							&out[out_index]);
				}
				out[out_index + 2] = UTF8_SPACE;
				out_index += 3;
			}

			out[out_index] = UTF8_SPACE;
			out_index += 1;
		}

		// ASCII representation column
		out[out_index] = UTF8_PIPE;
		out_index += 1;
		for (size_t j = 0; j < 16 && i + j < str_len;
				j += 1, out_index += 1) {
			if (str[i + j] < 0x7f && str[i + j] > 0x1f) {
				out[out_index] = str[i + j];
			} else {
				out[out_index] = UTF8_PERIOD;
			}
		}
		out[out_index]     = UTF8_PIPE;
		out[out_index + 1] = UTF8_NEWLINE;
		out_index += 2;
	}

	return 0;
}
