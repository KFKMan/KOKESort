#ifndef PARTS_HEAPSORT_H
#define PARTS_HEAPSORT_H

#include <sort.h>

#ifdef __cplusplus
extern "C" {  /* C++ name mangling */
#endif

KOKESORT_API void heapSort(void *arr, size_t n, size_t elementSize, CompareFunction cmp);

#ifdef __cplusplus
 };
#endif

#endif