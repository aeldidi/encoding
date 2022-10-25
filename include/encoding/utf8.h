// SPDX-License-Identifier: 0BSD
// Copyright (C) 2022 Ayman El Didi
#ifndef ENCODING_UTF8_H
#define ENCODING_UTF8_H

// encoding/utf8.h provides functions for encoding, decoding, and validating
// UTF-8 encoded text as defined in RFC 3629.
//
// RFC 3629 is included in the doc/ subdirectory of the source distribution,
// but can also be found at https://datatracker.ietf.org/doc/html/rfc3629

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "encoding/common.h"

#ifdef __cplusplus
extern "C" {
#endif

// utf8_valid checks if str is a valid UTF-8 encoded string, reading at most
// len bytes.
//
// On success, returns true.
// If str is NULL or invalid UTF-8, returns false.
ENCODING_PUBLIC
bool utf8_valid(const size_t str_len, const uint8_t* str);

// utf8_encode encodes the unicode codepoint sequence str into out, reading at
// most len bytes of str, and writing at most out_len bytes of out. Any invalid
// codepoints in str will be encoded as the Unicode Replacement Character
// (U+FFFD).
//
// On success, returns the number of bytes written.
// On failure, returns one of the negative error values below:
//
// ENCODING_INVALID_NULL_POINTER
// 	either str was NULL and str_len was greater than 0, or out was NULL and
// 	out_len was greater than 0.
//
// ENCODING_BUFFER_TOO_SMALL
// 	out_len was too small to hold the encoded data. To find out how many
//      bytes are needed to encode the data, call
//      utf8_encoded_length.
ENCODING_PUBLIC
int utf8_encode(const size_t str_len, const uint32_t* str,
		const size_t out_len, uint8_t* out);

// utf8_encoded_length returns the number of bytes str will take up when
// encoded as UTF-8. Invalid codepoints will be said to take up 3 bytes, since
// they are encoded as the Unicode Replacement Character (U+FFFD).
//
// On success, sets *out to the number of bytes str will take up when encoded
// as UTF-8 and returns 0.
// On failure, returns one of the negative error values below:
//
// ENCODING_INVALID_NULL_POINTER
// 	Either out was NULL or str was NULL and str_len was greater than 0.
ENCODING_PUBLIC
int utf8_encoded_length(
		const size_t str_len, const uint32_t* str, size_t* out);

// utf8_codepoint_encode encodes the codepoint cp into the string out writing
// at most out_len bytes. If cp is an invalid codepoint, the function tries to
// encode the Unicode Replacement Character (U+FFFD).
//
// On success, returns the number of bytes written.
// On failure, returns one of the negative error values below:
//
// ENCODING_INVALID_NULL_POINTER
// 	out was NULL and out_len was greater than 0.
//
// ENCODING_BUFFER_TOO_SMALL
//	out_len was too small to hold the encoded data. To find out how many
//	bytes are needed to encode the data, call
//	utf8_encoded_length.
ENCODING_PUBLIC
int utf8_codepoint_encode(
		const uint32_t codepoint, const size_t out_len, uint8_t* out);

// utf8_decode decodes the UTF-8 encoded string str into a sequence of Unicode
// codepoints out, reading at most len bytes of str and writing at most out_len
// codepoints to out. Any invalid bytes are decoded as the Unicode Replacement
// Character (U+FFFD).
//
// On success, returns 0.
// On failure, returns one of the negative error values below:
//
// ENCODING_INVALID_NULL_POINTER
//	either str was NULL and str_len was greater than 0, or out was NULL and
//	out_len was greater than 0.
//
// ENCODING_INVALID_ARGUMENT
//	A codepoint in str was truncated.
//
// ENCODING_BUFFER_TOO_SMALL
//	out_len was too small to hold the decoded data. To find out how many
//	bytes are needed to decode the data, call
//	utf8_decoded_length.
ENCODING_PUBLIC
int utf8_decode(const size_t str_len, const uint8_t* str, const size_t out_len,
		uint32_t* out);

// utf8_decoded_length counts the number of codepoints found in the first len
// bytes of str. Invalid UTF-8 is treated as a sequence of single byte
// codepoints.
//
// On success, sets *out to the number of codepoints in str and returns 0.
// On failure, returns one of the negative error values below:
//
// ENCODING_INVALID_NULL_POINTER
// 	Either out was NULL or str was NULL and str_len was greater than 0.
ENCODING_PUBLIC
int utf8_decoded_length(const size_t str_len, const uint8_t* str, size_t* out);

// utf8_codepoint_decode decodes the first codepoint found in str when reading
// at most len bytes. If size is not NULL, *size is set to the width of the
// read character.
//
// On success, returns the unicode codepoint number, i.e the number of the
// form `U+XXXXXX` for the decoded codepoint.
// On failure returns ENCODING_CODEPOINT_ERROR.
//
// Note that ENCODING_CODEPOINT_ERROR is simply the codepoint for the
// Unicode Replacement Character (U+FFFD), meaning in many cases errors do not
// necessarily need to be handled.
ENCODING_PUBLIC
uint32_t utf8_codepoint_decode(
		const size_t str_len, const uint8_t* str, size_t* size);

#ifdef __cplusplus
}
#endif

#endif // ENCODING_UTF8_H
