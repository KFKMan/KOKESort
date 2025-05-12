#ifndef PARTS_MERGESORT_H
#define PARTS_MERGESORT_H

#include <sort.h>

#ifdef __cplusplus
extern "C" {  /* C++ name mangling */
#endif

KOKESORT_API void mergeSortOpt(void *arr, size_t left, size_t right, size_t elementSize, CompareFunction cmp);
KOKESORT_API void mergeSort(void *arr, size_t n, size_t elementSize, CompareFunction cmp);

#ifdef __cplusplus
 };
#endif

#endif