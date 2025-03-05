#ifndef GLOBAL_H
#define GLOBAL_H

#define DEBUG

#ifdef DEBUG
#define ERROR_PRINT
#define debugPrint(fmt, ...) printf("[%s:%d] " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define debugError(fmt, ...) fprintf(stderr, "[%s:%d] " fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#endif

#ifdef _WIN32
    #ifdef KOKESORT_IMPORTS
        #define KOKESORT_API __declspec(dllimport)
    #elif defined(KOKESORT_EXPORTS)
        #define KOKESORT_API __declspec(dllexport)
    #else
        #define KOKESORT_API
    #endif
#else
    #define KOKESORT_API
#endif

#endif