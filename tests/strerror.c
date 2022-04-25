#include <string.h>

#include "common.h"
#include "encoding/common.h"

int
main()
{
	assert(strcmp(encoding_strerror(1), "Success") == 0);
	assert(strcmp(encoding_strerror(0), "Success") == 0);
	assert(strcmp(encoding_strerror(ENCODING_INVALID_NULL_POINTER),
			       "Invalid NULL pointer argument") == 0);
	assert(strcmp(encoding_strerror(ENCODING_INVALID_ARGUMENT),
			       "Invalid argument") == 0);
	assert(strcmp(encoding_strerror(ENCODING_BUFFER_TOO_SMALL),
			       "Length of output buffer too small") == 0);
	assert(strcmp(encoding_strerror(ENCODING_NOT_POWER_OF_TWO),
			       "Argument not power of 2 when required") == 0);
	assert(strcmp(encoding_strerror(-5), "Not an error the encoding "
					     "library can return") == 0);
}
