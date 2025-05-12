#ifndef PARTS_INSERTIONSORT_H
#define PARTS_INSERTIONSORT_H

#include <sort.h>

#ifdef __cplusplus
extern "C" {  /* C++ name mangling */
#endif

KOKESORT_API void insertionSort(void *arr, size_t n, size_t elementSize, CompareFunction cmp);

#ifdef __cplusplus
 };
#endif

#endif