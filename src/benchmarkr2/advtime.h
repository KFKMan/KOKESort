#ifndef BENCHMARKR2_ADVTIME_H
#define BENCHMARKR2_ADVTIME_H

#include "Global.h"

#ifdef WINDOWS
#include <Windows.h>
#else
#include <time.h>
#endif

long long get_time_ns();

#endif