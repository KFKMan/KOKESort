#ifndef KOKESort
#define KOKESort

#include "Global.h"
#include "Config.h"

#define USE_COMPARER

//Enabling Simple Compare Functions -1 (less), 0 (equal), 1 (more)
//#define COMPARE_USE_SIMPLE 1

#ifdef USE_COMPARER

typedef int (*CompareFunction)(const void *, const void *);

KOKESORT_API size_t FindInsertIndexBS(VariableType* arr, size_t size, VariableType element, CompareFunction comparer);

KOKESORT_API VariableType* InsertToSortedArray(VariableType* arr, size_t size, VariableType element, CompareFunction comparer);

KOKESORT_API void InsertToSortedAllocatedArray(VariableType* arr, size_t currentSize, VariableType element, CompareFunction comparer);

KOKESORT_API VariableType* SortV1(VariableType* arr, size_t size, CompareFunction comparer);

KOKESORT_API void FreeMemory(void* pointer);

#endif

#endif