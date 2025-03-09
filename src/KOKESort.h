#ifndef KOKESort
#define KOKESort

#include <string.h> //For memcpy

#include "Global.h"

#ifdef __cplusplus
 extern "C" {  /* C++ name mangling */
 #endif

#define USE_COMPARER

//Enabling Simple Compare Functions -1 (less), 0 (equal), 1 (more)
//#define COMPARE_USE_SIMPLE 1

#ifdef USE_COMPARER

#define VariableType void*
#define VariableTypeArray VariableType

typedef int (*CompareFunction)(const void *, const void *);

KOKESORT_API void* GetIndex(void* arr, size_t index, unsigned int elementSize);

KOKESORT_API int FindInsertIndexBS(VariableTypeArray arr, size_t size, VariableType element, CompareFunction comparer, unsigned int elementSize, size_t* insertIndex);

KOKESORT_API VariableTypeArray InsertToSortedArray(VariableTypeArray arr, size_t size, VariableType element, CompareFunction comparer, unsigned int elementSize);

KOKESORT_API int InsertToSortedAllocatedArray(VariableTypeArray arr, size_t currentSize, VariableType element, CompareFunction comparer, unsigned int elementSize);

KOKESORT_API VariableTypeArray SortV1(VariableTypeArray arr, size_t size, CompareFunction comparer, unsigned int elementSize);

KOKESORT_API void FreeMemory(void* pointer);

#endif

#ifdef __cplusplus
 };
#endif

#endif