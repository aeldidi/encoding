// SPDX-License-Identifier: 0BSD
// Copyright (C) 2022 Ayman El Didi
#ifndef ENCODING_HEX_H
#define ENCODING_HEX_H

// encoding/hex.h provides functions for encoding and decoding binary data into
// a string of hexadecimal characters. This is also known as base16.
//
// The functions hex_decoded_length and hex_decode
// both assume the input is a valid hex string, so be sure to validate their
// input using hex_valid before use.
//
// The accepted characters in a hex string are '0' to '9', 'a' to 'f', and 'A'
// to 'F'.

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

// hex_valid checks if the first str_len bytes of str only contain pairs of
// hexadecimal characters.
ENCODING_PUBLIC
bool hex_valid(const size_t str_len, const uint8_t* str);

// hex_encoded_length counts the number of bytes needed to encode len bytes as
// a hexadecimal string.
ENCODING_PUBLIC
size_t hex_encoded_length(const size_t len);

// hex_encode encodes the first str_len bytes of str into the first out_len
// bytes of out as a string of hexadecimal characters.
//
// out and str must not be NULL.
//
// On success, returns 0.
// On failure, returns one of the negative error values below:
//
// ENCODING_BUFFER_TOO_SMALL
// 	out_len was too small to hold the encoded data. To find out how many
//      bytes are needed to encode the data, call hex_encoded_length.
ENCODING_PUBLIC
int hex_encode(const size_t str_len, const uint8_t* str, const size_t out_len,
		uint8_t* out);

// hex_decoded_length counts the number of bytes needed to decode len bytes of
// a hexadecimal string into binary data.
//
// Assumes the input is a valid hex string length. That is, an incorrect value
// will be returned if len is not a multiple of 2.
ENCODING_PUBLIC
size_t hex_decoded_length(const size_t len);

// hex_decode decodes the first str_len bytes of the string of hexadecimal
// characters str into the first out_len bytes of out.
//
// Assumes the string in str is a valid string of hexadecimal characters. It is
// undefined behaviour what the result is when str is not valid. To check if
// str is valid, hex_valid should be called.
//
// out and str must not be NULL.
//
// On success, returns 0.
// On failure, returns one of the negative error values below:
//
// ENCODING_BUFFER_TOO_SMALL
// 	out_len was too small to hold the encoded data. To find out how many
//      bytes are needed to encode the data, call hex_encoded_length.
//
// ENCODING_INVALID_ARGUMENT
//	str_len is not a multiple of 2.
ENCODING_PUBLIC
int hex_decode(const size_t str_len, const uint8_t* str, const size_t out_len,
		uint8_t* out);

// hex_dump_length counts the number of bytes needed to contain len bytes in a
// formatted dump output produced by hex_dump.
ENCODING_PUBLIC
size_t hex_dump_length(const size_t len);

// hex_dump outputs a human-friendly UTF-8 representation of the first str_len
// bytes of str to the first out_len bytes of the buffer out.
//
// If offset != NULL, the value pointed to by offset will be used as the first
// address in the output, and the next address which would be printed had there
// been more input will be placed in *output.
//
// out and str must not be NULL.
//
// If offset is NULL, 0 will be displayed as the first address.
//
// On success, returns 0.
// On failure, returns one of the negative error values below:
//
// ENCODING_BUFFER_TOO_SMALL
// 	out_len was too small to hold the encoded data. To find out how many
//      bytes are needed to encode the data, call hex_dump_length.
//
// ENCODING_INVALID_ARGUMENT
//	The starting address specified in *offset would overflow given the
//	input data.
//
// Sample Output:
// ```
// 00000000  2a 2e 6f 62 6a 0a 2a 2e  65 78 65 0a 54 4f 44 4f  |*.obj.*.exe.TODO|
// 00000010  0a 62 75 69 6c 64 2a 0a  2e 63 61 63 68 65 2a     |.build*..cache*|
// ```
ENCODING_PUBLIC
int hex_dump(const size_t str_len, const uint8_t* str, const size_t out_len,
		uint8_t* out, uint64_t* offset);

#ifdef __cplusplus
}
#endif

#endif // ENCODING_HEX_H
