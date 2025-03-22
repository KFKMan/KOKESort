#ifndef GLOBAL_H
#define GLOBAL_H

#define USE_COMPARER

#include <stdio.h>
#include <stdlib.h>
#include "SecureFunctions.h"

#define SUCCESS 1
#define FAIL 0

#define TRUE 1
#define FALSE 0

#ifdef DEBUG
#define ERROR_PRINT
#define debugPrint(fmt, ...) printf("[%s:%d] " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define debugPrintFast(fmt, ...) do { printf("[%s:%d] " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); fflush(stdout); } while (1 == 0)

#define debugError(fmt, ...) fprintf(stderr, "[%s:%d] " fmt, __FILE__, __LINE__, ##__VA_ARGS__)

#define debugErrorFast(fmt, ...) do { fprintf(stderr, "[%s:%d] " fmt, __FILE__, __LINE__, ##__VA_ARGS__); fflush(stderr); } while (1 == 0)
#else
#define debugPrint(fmt, ...) 
#define debugPrintFast(fmt, ...) 
#define debugError(fmt, ...) 
#define debugErrorFast(fmt, ...) 
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

//https://stackoverflow.com/a/5920028
//https://stackoverflow.com/a/18729350
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
   //define something for Windows (32-bit and 64-bit, this part is common)
    #define WINDOWS

   #ifdef _WIN64
      //define something for Windows (64-bit only)
   #else
      //define something for Windows (32-bit only)
   #endif
#elif __APPLE__
    #include "TargetConditionals.h"
    #define APPLE
    #if TARGET_OS_IPHONE && TARGET_OS_SIMULATOR
        // define something for simulator
        // (although, checking for TARGET_OS_IPHONE should not be required).
    #elif TARGET_OS_IPHONE && TARGET_OS_MACCATALYST
        // define something for Mac's Catalyst
    #elif TARGET_OS_IPHONE
        // define something for iphone  
    #else
        #define TARGET_OS_OSX 1
        // define something for OSX
    #endif
#elif __ANDROID__
    // Below __linux__ check should be enough to handle Android,
    // but something may be unique to Android.
    #define ANDROID
#elif __linux__
    #define LINUX
#elif __unix__ // all unices not caught above
   #define UNIX
#elif defined(_POSIX_VERSION)
    // POSIX
    #define POSIX
#else
#error "Unknown compiler"
#endif

#ifdef USE_COMPARER
KOKESORT_API void* GetIndex(void* arr, size_t index, unsigned int elementSize);
KOKESORT_API void FreeMemory(void* pointer);

/// @brief First element, second element
typedef int (*CompareFunction)(const void *, const void *);

/// @brief Array, number of elements, size of one element, comparer 
typedef void (*SortFunction)(void *, size_t, size_t, CompareFunction);
#endif

#endif