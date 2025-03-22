#include "advtime.h"

#ifdef WINDOWS
long long get_time_ns() {
    LARGE_INTEGER frequency, counter;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&counter);

    return (counter.QuadPart * 1000000000L) / frequency.QuadPart;
}
#else
long long get_time_ns() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    return ts.tv_sec * 1000000000L + ts.tv_nsec;
}
#endif