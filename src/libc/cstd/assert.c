#include <assert.h>
#include <stdlib.h> // abort();
#include <stdio.h>

void
__assert(
    int val, const char *expr, const char *file, int line, const char *func)
{
        if (val) { return; }
        printf("assertion failed: %s at %s:%d:%s", expr, file, line, func);
	abort();
}
