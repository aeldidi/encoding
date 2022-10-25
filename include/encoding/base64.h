// SPDX-License-Identifier: 0BSD
// Copyright (C) 2022 Ayman El Didi
#ifndef ENCODING_BASE64_H
#define ENCODING_BASE64_H

// encoding/base64.h provides functions to encode and decode binary data in the
// formats described in RFC 4648.
//
// The base64_encode and base64_decode functions accept an
// alphabet parameter, which contains the bytes to use when encoding or
// decoding the input. Any 64-byte array can be used for this, but we provide
// base64 for use with the "base64" encoding defined in RFC 4648 and
// base64url for use with the "base64url" encoding defined in
// RFC 4648.
//
// RFC 4648 is included in the doc/ subdirectory of the source distribution,
// but can also be found at: https://datatracker.ietf.org/doc/html/rfc4648

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "encoding/common.h"

#ifdef __cplusplus
extern "C" {
#endif

// The alphabet for the "base64" encoding specified in RFC 4648.
ENCODING_PUBLIC
extern const uint8_t base64[64];

// The alphabet for the "base64url" encoding specified in RFC 4648.
ENCODING_PUBLIC
extern const uint8_t base64url[64];

// base64_encoded_length returns the base64 encoded size of the
// base64-encoded data with the length str_len.
ENCODING_PUBLIC
size_t base64_encoded_length(const size_t str_len);

// base64_valid returns true if str is valid base64, false otherwise.
ENCODING_PUBLIC
bool base64_valid(const size_t str_len, const uint8_t* str,
		const uint8_t alphabet[64]);

// base64_encode encodes str into base64, reading at most str_len bytes from
// str and writing at most out_len bytes to out.
//
// On success, returns 0.
// On failure, returns one of the following negative error codes:
//
// ENCODING_INVALID_NULL_POINTER
//	out was NULL and out_len was greater than 0, or str was NULL and
// 	str_len was greater than 0, or alphabet was NULL.
//
// ENCODING_BUFFER_TOO_SMALL
//	out_len was too small to encode the input data. To find out how large
//	the output buffer should be, call base64_encoded_length.
ENCODING_PUBLIC
int base64_encode(const size_t str_len, const uint8_t* str,
		const size_t out_len, uint8_t* out,
		const uint8_t alphabet[64]);

// base64_decoded_length gets the length of str decoded as base64. Reads at
// most str_len bytes from str.
//
// On success returns 0.
// If str or out is NULL returns ENCODING_INVALID_NULL_POINTER.
ENCODING_PUBLIC
int base64_decoded_length(
		const size_t str_len, const uint8_t* str, size_t* out);

// base64_decode decodes str as base64, reading at most str_len bytes from str
// and writing at most out_len bytes to out.
//
// On success, returns 0.
// On failure, returns one of the following negative error codes:
//
// ENCODING_INVALID_NULL_POINTER
//	out was NULL and out_len was greater than 0, or str was NULL and
// 	str_len was greater than 0, or alphabet was NULL.
//
// ENCODING_BUFFER_TOO_SMALL
//	out_len was too small to decode the input into. To find out how large
//	the output buffer should be, call base64_decoded_length.
//
// ENCODING_INVALID_ARGUMENT
//	str was not valid base64.
ENCODING_PUBLIC
int base64_decode(const size_t str_len, const uint8_t* str,
		const size_t out_len, uint8_t* out,
		const uint8_t alphabet[64]);

#ifdef __cplusplus
}
#endif

#endif // ENCODING_BASE64_H
