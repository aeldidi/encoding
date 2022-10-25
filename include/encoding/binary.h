// SPDX-License-Identifier: 0BSD
// Copyright (C) 2022 Ayman El Didi
#ifndef ENCODING_BINARY_H
#define ENCODING_BINARY_H

// encoding/binary.h provides functions for handling binary data of varying
// byte order.

#include <stddef.h>
#include <stdint.h>

#include "encoding/common.h"

#ifdef __cplusplus
extern "C" {
#endif

// binary_uint16_decode returns 16 decoded bits from a byte stream of the
// specified byte order.
//
// On success, returns 0 and sets *out to the decoded data.
// On failure, returns one of the following negative error codes:
//
// ENCODING_INVALID_NULL_POINTER
//	out was NULL, or bytes was NULL and bytes_len was greater than or equal
// 	to 2.
//
// ENCODING_BUFFER_TOO_SMALL
//	bytes_len is less than 2.
//
// ENCODING_INVALID_ARGUMENT
//	a value other than ENCODING_BYTE_ORDER_BIG or
// 	ENCODING_BYTE_ORDER_LITTLE was passed as byte_order.
ENCODING_PUBLIC
int binary_uint16_decode(const size_t bytes_len, const uint8_t* bytes,
		uint16_t* out, const enum byte_order byte_order);

// binary_uint16_encode encodes uint16 into the stream out using the specified
// byte order. Writes at most 2 bytes into out.
//
// On success, returns 0 and writes the encoded data to *out.
// On failure, returns one of the following negative error codes:
//
// ENCODING_INVALID_NULL_POINTER
//	out was NULL.
//
// ENCODING_BUFFER_TOO_SMALL
//	out_len was less than 2.
//
// ENCODING_INVALID_ARGUMENT
//	a value other than ENCODING_BYTE_ORDER_BIG or
// 	ENCODING_BYTE_ORDER_LITTLE was passed as byte_order.
ENCODING_PUBLIC
int binary_uint16_encode(const uint16_t uint16, const size_t out_len,
		uint8_t* out, const enum byte_order byte_order);

// binary_uint32_decode decodes 32 bits from a byte stream of the specified
// byte order.
//
// On success, returns 0 and sets *out to the decoded data.
// On failure, returns one of the following negative error codes:
//
// ENCODING_INVALID_NULL_POINTER
//	out was NULL, or bytes was NULL and bytes_len was greater than or equal
// 	to 2.
//
// ENCODING_BUFFER_TOO_SMALL
//	bytes_len is less than 2.
//
// ENCODING_INVALID_ARGUMENT
//	a value other than ENCODING_BYTE_ORDER_BIG or
// 	ENCODING_BYTE_ORDER_LITTLE was passed as byte_order.
ENCODING_PUBLIC
int binary_uint32_decode(const size_t bytes_len, const uint8_t* bytes,
		uint32_t* out, const enum byte_order byte_order);

// binary_uint32_encode encodes uint32 into the stream out using the specified
// byte order. Writes at most 4 bytes into out.
//
// On success, returns 0 and writes the encoded data to *out.
// On failure, returns one of the following negative error codes:
//
// ENCODING_INVALID_NULL_POINTER
//	out was NULL.
//
// ENCODING_BUFFER_TOO_SMALL
//	out_len was less than 2.
//
// ENCODING_INVALID_ARGUMENT
//	a value other than ENCODING_BYTE_ORDER_BIG or
// 	ENCODING_BYTE_ORDER_LITTLE was passed as byte_order.
ENCODING_PUBLIC
int binary_uint32_encode(const uint32_t uint32, const size_t out_len,
		uint8_t* out, const enum byte_order byte_order);

// binary_uint64_decode returns 64 decoded bits from a byte stream of the
// specified byte order.
//
// On success, returns 0 and sets *out to the decoded data.
// On failure, returns one of the following negative error codes:
//
// ENCODING_INVALID_NULL_POINTER
//	out was NULL, or bytes was NULL and bytes_len was greater than or equal
// 	to 2.
//
// ENCODING_BUFFER_TOO_SMALL
//	bytes_len is less than 2.
//
// ENCODING_INVALID_ARGUMENT
//	a value other than ENCODING_BYTE_ORDER_BIG or
// 	ENCODING_BYTE_ORDER_LITTLE was passed as byte_order.
ENCODING_PUBLIC
int binary_uint64_decode(const size_t bytes_len, const uint8_t* bytes,
		uint64_t* out, const enum byte_order byte_order);

// binary_uint64_encode encodes uint64 into the stream out using the specified
// byte order. Writes at most 8 bytes into out.
//
// On success, returns 0 and writes the encoded data to *out.
// On failure, returns one of the following negative error codes:
//
// ENCODING_INVALID_NULL_POINTER
//	out was NULL.
//
// ENCODING_BUFFER_TOO_SMALL
//	out_len was less than 2.
//
// ENCODING_INVALID_ARGUMENT
//	a value other than ENCODING_BYTE_ORDER_BIG or
// 	ENCODING_BYTE_ORDER_LITTLE was passed as byte_order.
ENCODING_PUBLIC
int binary_uint64_encode(const uint64_t uint64, const size_t out_len,
		uint8_t* out, const enum byte_order byte_order);

#ifdef __cplusplus
}
#endif

#endif // ENCODING_BINARY_H
