// SPDX-License-Identifier: 0BSD
// Copyright (C) 2022 Ayman El Didi
#ifndef ENCODING_BASE32_H
#define ENCODING_BASE32_H

// encoding/base32.h provides functions to encode and decode binary data in the
// formats described in RFC 4648.
//
// The base32_encode and base32_decode functions accept an
// alphabet parameter, which contains the bytes to use when encoding or
// decoding the input. Any 32-byte array can be used for this, but we provide
// base32 for use with the "base32" encoding defined in RFC 4648 and
// base32hex for use with the "base32hex" defined in RFC 4648. Also included is
// an alternative alphabet meant for human consumption and input, called
// base32crockford. The decoder for base32crockford is more forgiving.
//
// RFC 4648 is included in the doc/ subdirectory of the source distribution,
// but can also be found at: https://datatracker.ietf.org/doc/html/rfc4648

#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>

#if !defined(ENCODING_PUBLIC)
#define ENCODING_PUBLIC
#endif

#if !defined(ENCODING_INVALID_ARGUMENT)
#define ENCODING_INVALID_ARGUMENT (-2)
#endif

#if !defined(ENCODING_BUFFER_TOO_SMALL)
#define ENCODING_BUFFER_TOO_SMALL (-3)
#endif

#ifdef __cplusplus
extern "C" {
#endif

// The alphabet for the "base32" encoding specified in RFC 4648.
ENCODING_PUBLIC
extern const uint8_t base32[32];

// The alphabet for the "base32hex" encoding specified in RFC 4648.
ENCODING_PUBLIC
extern const uint8_t base32hex[32];

// The alphabet proposed by Douglas Crockford, designed to be human-readable.
// Source: https://www.crockford.com/base32.html
ENCODING_PUBLIC
extern const uint8_t base32crockford[32];

// base32_encoded_length returns the base32 encoded size of the
// base32-encoded data with the length str_len.
ENCODING_PUBLIC
size_t base32_encoded_length(const size_t str_len);

// base32_valid returns true if str is valid base32, false otherwise.
ENCODING_PUBLIC
bool base32_valid(const size_t str_len, const uint8_t* str,
		const uint8_t alphabet[32]);

// base32_encode encodes str into base32, reading at most str_len bytes from
// str and writing at most out_len bytes to out.
//
// out and str must not be NULL.
//
// On success, returns 0.
// On failure, returns one of the following negative error codes:
//
// ENCODING_BUFFER_TOO_SMALL
//	out_len was too small to encode the input data. To find out how large
//	the output buffer should be, call base32_encoded_length.
ENCODING_PUBLIC
int base32_encode(const size_t str_len, const uint8_t* str,
		const size_t out_len, uint8_t* out,
		const uint8_t alphabet[32]);

// base32_decoded_length gets the length of str decoded as base32. Reads at
// most str_len bytes from str.
//
// str must be NULL.
ENCODING_PUBLIC
size_t base32_decoded_length(const size_t str_len, const uint8_t* str);

// base32_decode decodes str as base32, reading at most str_len bytes from str
// and writing at most out_len bytes to out.
//
// out and str must not be NULL.
//
// On success, returns 0.
// On failure, returns one of the following negative error codes:
//
// ENCODING_BUFFER_TOO_SMALL
//	out_len was too small to decode the input into. To find out how large
//	the output buffer should be, call base32_decoded_length.
//
// ENCODING_INVALID_ARGUMENT
//	str was not valid base32.
ENCODING_PUBLIC
int base32_decode(const size_t str_len, const uint8_t* str,
		const size_t out_len, uint8_t* out,
		const uint8_t alphabet[32]);

#ifdef __cplusplus
}
#endif

#endif // ENCODING_BASE32_H
