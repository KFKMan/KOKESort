#ifndef PARTS_INSERTIONSORT_H
#define PARTS_INSERTIONSORT_H

#include <sort.h>

KOKESORT_API void insertionSort(void *arr, size_t n, size_t elementSize, CompareFunction cmp);

#endif