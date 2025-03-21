#ifndef PARTS_MASTER_H
#define PARTS_MASTER_H

#include "bubblesort.h"
#include "heapsort.h"
#include "insertionsort.h"
#include "mergesort.h"
#include "quicksort.h"
#include "selectionsort.h"

typedef struct
{
    const char *name;
    SortFunction function;
} SortFunctionEntry;

#define _SortingFunctions {{"BubbleSort", bubbleSort},{"HeapSort", heapSort},{"InsertionSort", insertionSort},{"MergeSort", mergeSort},{"QuickSort", quickSort},{"SelectionSort", selectionSort},}

#endif