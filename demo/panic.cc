#include "panic.h"

#include <cstdio>
#include <cstdlib>
#include <cstdarg>

void panic(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    static char buf[64*1024];
    vsprintf(buf, fmt, ap);
    va_end(ap);

    fprintf(stderr, "%s\n", buf);

    abort();
}
