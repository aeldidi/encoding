// SPDX-License-Identifier: 0BSD
// Copyright (C) 2022 Ayman El Didi
#include <stdio.h>

int
main()
{
	for (int i = 0; i < 256; i += 1) {
		if (i >= 0xc2 && i <= 0xdf) {
			printf("2,\n");
			continue;
		} else if (i >= 0xe0 && i <= 0xef) {
			printf("3,\n");
			continue;
		} else if (i >= 0xf0 && i <= 0xf4) {
			printf("4,\n");
			continue;
		}

		printf("1,\n");
	}

	return 0;
}
