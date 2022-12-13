// SPDX-License-Identifier: 0BSD
// Copyright (C) 2022 Ayman El Didi
#include <assert.h>
#include <inttypes.h>
#include <limits.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Inlines all #includes which use quotation marks ("") ignoring includes which
// use angle brackets (<>). Given a filename without an extension, outputs an
// amalgamated .c and .h file.
//
// All includes are relative to the file being amalgamated.

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
		fclose(f);
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
			fclose(f);
			return NULL;
		}
		result = ptr;
	}

	fclose(f);
	return result;
}

void
amalgamate_file(char* dir, char* file)
{
	FILE* f = fopen(file, "w");
	assert(f != NULL);

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

		char* fname = format("%s/%.*s", dir, closing_quote - ptr, ptr);
		assert(fname != NULL);
		char* include = (char*)readfull(fname);
		assert(include != NULL);
		free(fname);
		(void)fprintf(f, "%s\n", include);
		free(include);

		prev = closing_quote + 1;
	}

	(void)fprintf(f, "%s\n", prev);
	free(content);

	fclose(f);
}

int
main(int argc, char** argv)
{
	if (argc != 4) {
		(void)fprintf(stderr,
				"usage: %s [.c directory] [.h directory] "
				"[filename without "
				"extension]\n"
				"Given a source filename without its "
				"extension, amalgamates its .c source and its "
				"header.\n\n"
				"All includes are resolved relative to the "
				"file being amalgamated.\n",
				argv[0]);
		return EXIT_FAILURE;
	}

	char* fname = format("%s.c", argv[3]);
	assert(fname != NULL);

	amalgamate_file(argv[1], fname);

	free(fname);

	fname = format("%s.h", argv[3]);
	assert(fname != NULL);

	amalgamate_file(argv[2], fname);

	free(fname);
}
