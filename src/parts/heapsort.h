#ifndef PARTS_HEAPSORT_H
#define PARTS_HEAPSORT_H

#include <sort.h>

KOKESORT_API void heapSort(void *arr, size_t n, size_t elementSize, CompareFunction cmp);

#endif