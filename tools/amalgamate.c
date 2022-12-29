// SPDX-License-Identifier: 0BSD
// Copyright (C) 2022 Ayman El Didi
#include <assert.h>
#include <inttypes.h>
#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Inlines all #includes which use quotation marks ("") ignoring includes which
// use angle brackets (<>). Given a filename without an extension, outputs an
// amalgamated .c and .h file.
//
// All includes are relative to the file being amalgamated.

#define MAX(x, y) ((x) > (y) ? (x) : (y))

#define FNV_64_PRIME   UINT64_C(0x100000001b3)
#define FNV_64_INITIAL UINT64_C(0xcbf29ce484222325)

uint64_t
fnv_1a(const size_t buf_len, const uint8_t* buf)
{
	uint64_t hash = FNV_64_INITIAL;

	for (size_t i = 0; i < buf_len; i += 1) {
		hash = (hash ^ buf[i]) * FNV_64_PRIME;
	}

	return hash;
}

static char*
vformat(const char* fmt, va_list args)
{
	// The v*printf functions destroy the va_list after use.
	// So I have to copy the va_list for the second call or we segfault.
	va_list ap;
	va_copy(ap, args);
	int len = vsnprintf(NULL, 0, fmt, args);
	if (len < 0 || len == INT_MAX) {
		return NULL;
	}

	char* result = calloc(len + 1, 1);
	if (result == NULL) {
		return NULL;
	}

	(void)vsnprintf(result, len + 1, fmt, ap);
	va_end(ap);
	return result;
}

static char*
format(const char* fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	char* result = vformat(fmt, ap);
	va_end(ap);
	return result;
}

static uint8_t*
readfull(char* filename)
{
	FILE* f = fopen(filename, "r");
	if (f == NULL) {
		return NULL;
	}

	uint8_t* result = calloc(2, 1);
	if (result == NULL) {
		(void)fclose(f);
		return result;
	}

	int    c = 0;
	size_t i = 0;
	for (;;) {
		c = fgetc(f);
		if (c == EOF) {
			break;
		}
		result[i]     = c & 0xff;
		result[i + 1] = '\0';
		i += 1;
		void* ptr = realloc(result, i + 2);
		if (ptr == NULL) {
			free(result);
			(void)fclose(f);
			return NULL;
		}
		result = ptr;
	}

	(void)fclose(f);
	return result;
}

#define MAX_INCLUDES 1000

bool
file_has_been_included(const size_t str_len, const char* str,
		const size_t hashes_len, const uint64_t* hashes)
{
	uint64_t hash = fnv_1a(str_len, (uint8_t*)str);
	for (size_t i = 0; i < hashes_len; i += 1) {
		if (hash == hashes[i]) {
			return true;
		}
	}

	return false;
}

// Amalgamates file in dir into f, using pool to check if a file has already
// been included.
void
amalgamate_file(const char* dir, const char* file, FILE* f, size_t* hashes_len,
		uint64_t* hashes)
{
	char* filename = format("%s/%s", dir, file);
	assert(filename != NULL);
	char* content = (char*)readfull(filename);
	assert(content != NULL);
	free(filename);

	char* prev = content;
	char* ptr  = content;
	while ((ptr = strstr(ptr, "#include \"")) != NULL) {
		(void)fprintf(f, "%.*s", (int)(ptr - prev), prev);
		ptr += strlen("#include \"");

		char* closing_quote = strchr(ptr, '"');
		assert(closing_quote != NULL);

		size_t include_len = closing_quote - ptr;
		if (file_has_been_included(
				    include_len, ptr, *hashes_len, hashes)) {
			prev = closing_quote + 2;
			continue;
		}

		assert(*hashes_len + 1 < MAX_INCLUDES);
		hashes[*hashes_len] = fnv_1a(include_len, (uint8_t*)ptr);
		*hashes_len += 1;

		char* fname = format("%s/%.*s", dir, closing_quote - ptr, ptr);
		assert(fname != NULL);
		char* include = (char*)readfull(fname);
		assert(include != NULL);
		free(fname);
		(void)fprintf(f, "%s\n", include);
		free(include);

		prev = closing_quote + 2;
	}

	(void)fprintf(f, "%s", prev);
	free(content);
}

int
main(int argc, char** argv)
{
	if (argc != 3) {
		(void)fprintf(stderr,
				"usage: %s [src directory] "
				"[filename without extension]\n"
				"Given a source filename without its "
				"extension, amalgamates its .c source and its "
				"header.\n\n"
				"All includes are resolved relative to the "
				"src directory given.\n",
				argv[0]);
		return EXIT_FAILURE;
	}

	size_t src_len = strlen(argv[1]);
	if (argv[1][src_len - 1] == '/') {
		argv[1][src_len - 1] = '\0';
	}

	char* fname = format("%s.c", argv[2]);
	assert(fname != NULL);
	FILE* f = fopen(fname, "w");
	assert(f != NULL);

	static uint64_t include_hashes[MAX_INCLUDES];
	size_t          include_hashes_len = 0;

	amalgamate_file(argv[1], fname, f, &include_hashes_len,
			include_hashes);

	free(fname);
	(void)fclose(f);

	fname = format("%s.h", argv[2]);
	assert(fname != NULL);
	f = fopen(fname, "w");
	assert(f != NULL);

	amalgamate_file(argv[1], fname, f, &include_hashes_len,
			include_hashes);

	free(fname);
	(void)fclose(f);
}
