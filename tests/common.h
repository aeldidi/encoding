#ifndef TEST_COMMON_H
#define TEST_COMMON_H

#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define ARRAY_SIZEOF(x) (sizeof((x)) / sizeof(*(x)))

#define assert(cond)                                                          \
	do {                                                                  \
		if (!(cond)) {                                                \
			printf("failure on line %u\n", __LINE__);             \
			return EXIT_FAILURE;                                  \
		}                                                             \
	} while (0)

static inline bool
mem_equal(const uint8_t* a, const uint8_t* b, const size_t n)
{
	bool result = true;
	for (size_t i = 0; i < n; i += 1) {
		if (a[i] != b[i]) {
			result = false;
		}
	}

	return result;
}

static inline void
mem_set(uint8_t* dest, const uint8_t byte, const size_t n)
{
	for (size_t i = 0; i < n; i += 1) {
		dest[i] = byte;
	}
}

#endif // TEST_COMMON_H
