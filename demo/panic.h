#pragma once

#include <cstdio>
#include <cstdlib>

inline void panic(const char *fmt)
{
    fprintf(stderr, "%s", fmt);
    abort();
}

template <typename... Args>
inline void panic(const char *fmt, const Args &... args)
{
    fprintf(stderr, fmt, args...);
    abort();
}
