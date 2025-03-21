#ifndef PARTS_SELECTIONSORT_H
#define PARTS_SELECTIONSORT_H

#include <sort.h>

KOKESORT_API void selectionSort(void *arr, size_t n, size_t elementSize, CompareFunction cmp);

#endif