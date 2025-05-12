#ifndef PARTS_SELECTIONSORT_H
#define PARTS_SELECTIONSORT_H

#include <sort.h>

#ifdef __cplusplus
extern "C"
{ /* C++ name mangling */
#endif

KOKESORT_API void selectionSort(void *arr, size_t n, size_t elementSize, CompareFunction cmp);

#ifdef __cplusplus
 };
#endif

#endif