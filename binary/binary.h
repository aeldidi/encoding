// SPDX-License-Identifier: 0BSD
// Copyright (C) 2022 Ayman El Didi
#ifndef ENCODING_BINARY_H
#define ENCODING_BINARY_H

// encoding/binary.h provides functions for handling binary data of varying
// byte order. You should include this C file directly, as it only contains
// static functions.

#include <assert.h>
#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>

// Finding the C version

#if !defined(ENCODING_C23)
#define ENCODING_C23 0
#endif

#if defined(__STDC_VERSION__)
#if __STDC_VERSION__ >= 202311L
#undef ENCODING_C23
#define ENCODING_C23 1
#endif
#endif // defined(__STDC_VERSION__)

#if !defined(ENCODING_CPP17)
#define ENCODING_CPP17 0
#endif

#if defined(__cplusplus)
#if __cplusplus >= 201703L
#undef ENCODING_CPP17
#define ENCODING_CPP17 1
#endif
#endif

// Finding if [[maybe_unused]] or __atribute__((unused)) is supported

#if !defined(__has_c_attribute)
#define __has_c_attribute(x) 0
#endif

#if !defined(__has_attribute)
#define __has_attribute(x) 0
#endif

#if !defined(ENCODING_UNUSED)
#define ENCODING_UNUSED
#endif

#if (__has_c_attribute(maybe_unused) && ENCODING_C23) || ENCODING_CPP17
#undef ENCODING_UNUSED
#define ENCODING_UNUSED [[maybe_unused]]
#elif __has_attribute(unused)
#undef ENCODING_UNUSED
#define ENCODING_UNUSED __attribute__((unused))
#endif // (__has_c_attribute(maybe_unused) && ENCODING_C23) || ENCODING_CPP17

#if !defined(ENCODING_BYTE_ORDER_BIG)
#define ENCODING_BYTE_ORDER_BIG (0)
#endif

#if !defined(ENCODING_BYTE_ORDER_LITTLE)
#define ENCODING_BYTE_ORDER_LITTLE (1)
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

// binary_uint16_decode returns 16 decoded bits from a byte stream of the
// specified byte order.
//
// If something other than ENCODING_BYTE_ORDER_BIG or
// ENCODING_BYTE_ORDER_LITTLE is passed as byte_order, then undefined behaviour
// occurs.
//
// On success, returns 0 and sets *out to the decoded data.
// On failure, returns one of the following negative error codes:
//
// ENCODING_BUFFER_TOO_SMALL
//	bytes_len is less than 2.
ENCODING_UNUSED
static int
binary_uint16_decode(const size_t bytes_len, const uint8_t* bytes,
		uint16_t* out, const int byte_order)
{
	assert(out != NULL);
	assert(bytes != NULL);
	assert(byte_order == ENCODING_BYTE_ORDER_BIG ||
			byte_order == ENCODING_BYTE_ORDER_LITTLE);

	if (bytes_len < 2) {
		return ENCODING_BUFFER_TOO_SMALL;
	}

	if (byte_order == ENCODING_BYTE_ORDER_BIG) {
		*out = (uint16_t)(0xff & bytes[0]) << 8 |
		       (uint16_t)(0xff & bytes[1]) << 0;
		return 0;
	}

	*out = (uint16_t)(0xff & bytes[1]) << 8 | (uint16_t)(0xff & bytes[0])
								  << 0;
	return 0;
}

// binary_uint16_encode encodes uint16 into the stream out using the specified
// byte order. Writes at most 2 bytes into out.
//
// On success, returns 0 and writes the encoded data to *out.
// On failure, returns one of the following negative error codes:
//
// ENCODING_BUFFER_TOO_SMALL
//	out_len was less than 2.
ENCODING_UNUSED
static int
binary_uint16_encode(const uint16_t uint16, const size_t out_len, uint8_t* out,
		const int byte_order)
{
	assert(out != NULL);
	assert(byte_order == ENCODING_BYTE_ORDER_BIG ||
			byte_order == ENCODING_BYTE_ORDER_LITTLE);

	if (out_len < 2) {
		return ENCODING_BUFFER_TOO_SMALL;
	}

	if (byte_order == ENCODING_BYTE_ORDER_BIG) {
		out[0] = (uint8_t)((0x000000000000ff00 & uint16) >> 8);
		out[1] = (uint8_t)((0x00000000000000ff & uint16) >> 0);
		return 0;
	}

	out[0] = (uint8_t)((0x00000000000000ff & uint16) >> 0);
	out[1] = (uint8_t)((0x000000000000ff00 & uint16) >> 8);
	return 0;
}

// binary_uint32_decode decodes 32 bits from a byte stream of the specified
// byte order.
//
// out and bytes must not be NULL.
//
// On success, returns 0 and sets *out to the decoded data.
// On failure, returns one of the following negative error codes:
//
// ENCODING_BUFFER_TOO_SMALL
//	bytes_len is less than 2.
//
// ENCODING_INVALID_ARGUMENT
//	a value other than ENCODING_BYTE_ORDER_BIG or
// 	ENCODING_BYTE_ORDER_LITTLE was passed as byte_order.
ENCODING_UNUSED
static int
binary_uint32_decode(const size_t bytes_len, const uint8_t* bytes,
		uint32_t* out, const int byte_order)
{
	assert(out != NULL);
	assert(bytes != NULL);
	assert(byte_order == ENCODING_BYTE_ORDER_BIG ||
			byte_order == ENCODING_BYTE_ORDER_LITTLE);

	if (bytes_len < 4) {
		return ENCODING_BUFFER_TOO_SMALL;
	}

	if (byte_order == ENCODING_BYTE_ORDER_BIG) {
		*out = (uint32_t)(0xff & bytes[0]) << 24 |
		       (uint32_t)(0xff & bytes[1]) << 16 |
		       (uint32_t)(0xff & bytes[2]) << 8 |
		       (uint32_t)(0xff & bytes[3]) << 0;
		return 0;
	}

	*out = (uint32_t)(0xff & bytes[3]) << 24 |
	       (uint32_t)(0xff & bytes[2]) << 16 |
	       (uint32_t)(0xff & bytes[1]) << 8 |
	       (uint32_t)(0xff & bytes[0]) << 0;
	return 0;
}

// binary_uint32_encode encodes uint32 into the stream out using the specified
// byte order. Writes at most 4 bytes into out.
//
// out must not be NULL.
//
// On success, returns 0 and writes the encoded data to *out.
// On failure, returns one of the following negative error codes:
//
// ENCODING_BUFFER_TOO_SMALL
//	out_len was less than 2.
//
// ENCODING_INVALID_ARGUMENT
//	a value other than ENCODING_BYTE_ORDER_BIG or
// 	ENCODING_BYTE_ORDER_LITTLE was passed as byte_order.
ENCODING_UNUSED
static int
binary_uint32_encode(const uint32_t uint32, const size_t out_len, uint8_t* out,
		const int byte_order)
{
	assert(out != NULL);
	assert(byte_order == ENCODING_BYTE_ORDER_BIG ||
			byte_order == ENCODING_BYTE_ORDER_LITTLE);

	if (out_len < 4) {
		return ENCODING_BUFFER_TOO_SMALL;
	}

	if (byte_order == ENCODING_BYTE_ORDER_BIG) {
		out[0] = (uint8_t)((0x00000000ff000000 & uint32) >> 24);
		out[1] = (uint8_t)((0x0000000000ff0000 & uint32) >> 16);
		out[2] = (uint8_t)((0x000000000000ff00 & uint32) >> 8);
		out[3] = (uint8_t)((0x00000000000000ff & uint32) >> 0);
		return 0;
	}

	out[0] = (uint8_t)((0x00000000000000ff & uint32) >> 0);
	out[1] = (uint8_t)((0x000000000000ff00 & uint32) >> 8);
	out[2] = (uint8_t)((0x0000000000ff0000 & uint32) >> 16);
	out[3] = (uint8_t)((0x00000000ff000000 & uint32) >> 24);
	return 0;
}

// binary_uint64_decode returns 64 decoded bits from a byte stream of the
// specified byte order.
//
// out and bytes mut not be NULL.
//
// On success, returns 0 and sets *out to the decoded data.
// On failure, returns one of the following negative error codes:
//
// ENCODING_BUFFER_TOO_SMALL
//	bytes_len is less than 2.
//
// ENCODING_INVALID_ARGUMENT
//	a value other than ENCODING_BYTE_ORDER_BIG or
// 	ENCODING_BYTE_ORDER_LITTLE was passed as byte_order.
ENCODING_UNUSED
static int
binary_uint64_decode(const size_t bytes_len, const uint8_t* bytes,
		uint64_t* out, const int byte_order)
{
	assert(out != NULL);
	assert(bytes != NULL);
	assert(byte_order == ENCODING_BYTE_ORDER_BIG ||
			byte_order == ENCODING_BYTE_ORDER_LITTLE);

	if (bytes_len < 8) {
		return ENCODING_BUFFER_TOO_SMALL;
	}

	if (byte_order == ENCODING_BYTE_ORDER_BIG) {
		*out = (uint64_t)(0xff & bytes[0]) << 56 |
		       (uint64_t)(0xff & bytes[1]) << 48 |
		       (uint64_t)(0xff & bytes[2]) << 40 |
		       (uint64_t)(0xff & bytes[3]) << 32 |
		       (uint64_t)(0xff & bytes[4]) << 24 |
		       (uint64_t)(0xff & bytes[5]) << 16 |
		       (uint64_t)(0xff & bytes[6]) << 8 |
		       (uint64_t)(0xff & bytes[7]) << 0;
		return 0;
	}

	*out = (uint64_t)(0xff & bytes[7]) << 56 |
	       (uint64_t)(0xff & bytes[6]) << 48 |
	       (uint64_t)(0xff & bytes[5]) << 40 |
	       (uint64_t)(0xff & bytes[4]) << 32 |
	       (uint64_t)(0xff & bytes[3]) << 24 |
	       (uint64_t)(0xff & bytes[2]) << 16 |
	       (uint64_t)(0xff & bytes[1]) << 8 |
	       (uint64_t)(0xff & bytes[0]) << 0;
	return 0;
}

// binary_uint64_encode encodes uint64 into the stream out using the specified
// byte order. Writes at most 8 bytes into out.
//
// out must not be NULL.
//
// On success, returns 0 and writes the encoded data to *out.
// On failure, returns one of the following negative error codes:
//
// ENCODING_BUFFER_TOO_SMALL
//	out_len was less than 2.
//
// ENCODING_INVALID_ARGUMENT
//	a value other than ENCODING_BYTE_ORDER_BIG or
// 	ENCODING_BYTE_ORDER_LITTLE was passed as byte_order.
ENCODING_UNUSED
static int
binary_uint64_encode(const uint64_t uint64, const size_t out_len, uint8_t* out,
		const int byte_order)
{
	assert(out != NULL);
	assert(byte_order == ENCODING_BYTE_ORDER_BIG ||
			byte_order == ENCODING_BYTE_ORDER_LITTLE);

	if (out_len < 8) {
		return ENCODING_BUFFER_TOO_SMALL;
	}

	if (byte_order == ENCODING_BYTE_ORDER_BIG) {
		out[0] = (uint8_t)((0xff00000000000000 & uint64) >> 56);
		out[1] = (uint8_t)((0x00ff000000000000 & uint64) >> 48);
		out[2] = (uint8_t)((0x0000ff0000000000 & uint64) >> 40);
		out[3] = (uint8_t)((0x000000ff00000000 & uint64) >> 32);
		out[4] = (uint8_t)((0x00000000ff000000 & uint64) >> 24);
		out[5] = (uint8_t)((0x0000000000ff0000 & uint64) >> 16);
		out[6] = (uint8_t)((0x000000000000ff00 & uint64) >> 8);
		out[7] = (uint8_t)((0x00000000000000ff & uint64) >> 0);
		return 0;
	}

	out[0] = (uint8_t)((0x00000000000000ff & uint64) >> 0);
	out[1] = (uint8_t)((0x000000000000ff00 & uint64) >> 8);
	out[2] = (uint8_t)((0x0000000000ff0000 & uint64) >> 16);
	out[3] = (uint8_t)((0x00000000ff000000 & uint64) >> 24);
	out[4] = (uint8_t)((0x000000ff00000000 & uint64) >> 32);
	out[5] = (uint8_t)((0x0000ff0000000000 & uint64) >> 40);
	out[6] = (uint8_t)((0x00ff000000000000 & uint64) >> 48);
	out[7] = (uint8_t)((0xff00000000000000 & uint64) >> 56);
	return 0;
}

#ifdef __cplusplus
}
#endif

#endif // ENCODING_BINARY_H
