#include "quicksort.h"


size_t partition(void *arr, size_t low, size_t high, size_t elementSize, CompareFunction cmp) 
{
    void* pivot = GetIndex(arr, high, elementSize);
    size_t i = low;
    for (size_t j = low; j < high; j++) 
    {
        if (cmp(GetIndex(arr, j, elementSize), pivot) < 0) 
        {
            void* a = GetIndex(arr, i, elementSize);
            void* b = GetIndex(arr, j, elementSize);
            void* temp = malloc(elementSize);
            memcpy(temp, a, elementSize);
            memcpy(a, b, elementSize);
            memcpy(b, temp, elementSize);
            free(temp);
            i++;
        }
    }
    void* a = GetIndex(arr, i, elementSize);
    void* b = GetIndex(arr, high, elementSize);
    void* temp = malloc(elementSize);
    memcpy(temp, a, elementSize);
    memcpy(a, b, elementSize);
    memcpy(b, temp, elementSize);
    free(temp);
    return i;
}

void quickSortBasic(void *arr, size_t low, size_t high, size_t elementSize, CompareFunction cmp) 
{
    if (low < high) {
        size_t pi = partition(arr, low, high, elementSize, cmp);
        quickSortBasic(arr, low, pi - 1, elementSize, cmp);
        quickSortBasic(arr, pi + 1, high, elementSize, cmp);
    }
}