// SPDX-License-Identifier: 0BSD
// Copyright (C) 2022 Ayman El Didi
#include <stdio.h>

int
main()
{
	for (int i = 0; i < 0x67; i += 1) {
		if (i >= 0x30 && i <= 0x39) {
			printf("%d,\n", i - 0x30);
			continue;
		}

		if (i >= 0x61 && i <= 0x66) {
			printf("%d,\n", i - 87);
			continue;
		}

		if (i >= 0x41 && i <= 0x46) {
			printf("%d,\n", i - 55);
			continue;
		}

		printf("0,\n");
	}

	return 0;
}
