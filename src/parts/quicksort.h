#ifndef PARTS_QUICKSORT_H
#define PARTS_QUICKSORT_H

#include <sort.h>

#ifdef __cplusplus
extern "C" {  /* C++ name mangling */
#endif

#define quickSort qsort

KOKESORT_API void quickSortBasic(void *arr, size_t low, size_t high, size_t elementSize, CompareFunction cmp);

#ifdef __cplusplus
 };
#endif

#endif