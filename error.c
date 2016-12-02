#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "error.h"

void fatalError(const char *error, ...) {
    va_list args;
    va_start(args, error);
    vfprintf(stderr, error, args);
    va_end(args);
    fprintf(stderr, "\n");
    exit(1);
}
