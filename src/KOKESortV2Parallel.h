#ifndef KOKESORTV2Parallel
#define KOKESORTV2Parallel
#include "KOKESortV2.h"

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "cthreads.h"

#define INFINTE_JOIN_TIMEOUT LONG_INFINITE;

typedef struct cthreads_thread cthread_t;
typedef struct cthreads_args cthread_args;

#ifdef _MSC_VER
    #include <windows.h>

    typedef volatile LONG k_atomic_size_t;
    typedef volatile BOOL k_atomic_bool;

    #define ATOMIC_ADD_SIZE_T(ptr, val) InterlockedAdd((k_atomic_size_t*)(ptr), (val))
    #define ATOMIC_STORE_SIZE_T(ptr, val) InterlockedExchange((k_atomic_size_t*)(ptr), (val))
    #define ATOMIC_LOAD_SIZE_T(ptr) (*(ptr))
#else
    #include <stdatomic.h>

    typedef _Atomic size_t k_atomic_size_t;
    typedef _Atomic bool k_atomic_bool;

    #define ATOMIC_ADD_SIZE_T(ptr, val) atomic_fetch_add((k_atomic_size_t*)(ptr), (val))
    #define ATOMIC_STORE_SIZE_T(ptr, val) atomic_store((k_atomic_size_t*)(ptr), (val))
    #define ATOMIC_LOAD_SIZE_T(ptr) atomic_load((k_atomic_size_t*)(ptr))
#endif

typedef struct CountThreadArgs
{
    void *startPoint;                       // Pointer to the start of the array section this thread will process
    size_t processCount;                    // Number of elements to process
    size_t elementSize;                     // Size of each element in bytes

    PossibilitySpace *pbSpaces;            
    size_t pbSpaceCount;                    // Maximum number of spaces (limit of indexer result)
    SpaceIndexerFn indexerFn;               // Function that extracts space index from element
    void* indexerFnData;

    k_atomic_bool *errorFlag;               // Shared error flag
} CountThreadArgs;

typedef struct {
    void* startPoint;
    size_t processCount;
    size_t elementSize;

    
    PossibilitySpace* pbSpaces;
    size_t pbSpaceCount;

    SpaceIndexerFn indexerFn;
    void* indexerFnData;
} CopyThreadArgs;

KOKESORT_API PossibilitySpace* SortV2Parallel(void *array, size_t arraySize, size_t elementSize, size_t pbSpaceCount, SpaceIndexerFn indexerFn, void* indexerFnData, CompareFunction comparerFn, SortFunction sortFn, size_t threadCount, unsigned long threadTimeoutMs);

#endif