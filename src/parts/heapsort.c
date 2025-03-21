#include "heapsort.h"

void heapify(void *arr, size_t n, size_t i, size_t elementSize, CompareFunction cmp) 
{
    size_t largest = i;
    size_t left = 2 * i + 1;
    size_t right = 2 * i + 2;

    if (left < n && cmp(GetIndex(arr, left, elementSize), GetIndex(arr, largest, elementSize)) > 0) 
    {
        largest = left;
    }

    if (right < n && cmp(GetIndex(arr, right, elementSize), GetIndex(arr, largest, elementSize)) > 0) 
    {
        largest = right;
    }

    if (largest != i) 
    {
        void* a = GetIndex(arr, i, elementSize);
        void* b = GetIndex(arr, largest, elementSize);
        void* temp = malloc(elementSize);
        memcpy(temp, a, elementSize);
        memcpy(a, b, elementSize);
        memcpy(b, temp, elementSize);
        free(temp);

        heapify(arr, n, largest, elementSize, cmp);
    }
}

void heapSort(void *arr, size_t n, size_t elementSize, CompareFunction cmp) 
{
    for (size_t i = n / 2 - 1; i < n; i--) 
    {
        heapify(arr, n, i, elementSize, cmp);
    }

    for (size_t i = n - 1; i > 0; i--) 
    {
        void* a = GetIndex(arr, 0, elementSize);
        void* b = GetIndex(arr, i, elementSize);
        void* temp = malloc(elementSize);
        memcpy(temp, a, elementSize);
        memcpy(a, b, elementSize);
        memcpy(b, temp, elementSize);
        free(temp);

        heapify(arr, i, 0, elementSize, cmp);
    }
}