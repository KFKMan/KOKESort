#include "selectionsort.h"

void selectionSort(void *arr, size_t n, size_t elementSize, CompareFunction cmp) 
{
    void* temp = malloc(elementSize);
    if (!temp) 
    {
         //TODO: Throw Exception or Return Fail Status
        return;
    }

    for (size_t i = 0; i < n - 1; i++) 
    {
        size_t minIndex = i;
        for (size_t j = i + 1; j < n; j++) 
        {
            void* a = GetIndex(arr, j, elementSize);
            void* b = GetIndex(arr, minIndex, elementSize);
            
            if (cmp(a, b) < 0) 
            {
                minIndex = j;
            }
        }

        if (minIndex != i) 
        {
            void* a = GetIndex(arr, i, elementSize);
            void* b = GetIndex(arr, minIndex, elementSize);
            memcpy(temp, a, elementSize);
            memcpy(a, b, elementSize);
            memcpy(b, temp, elementSize);
        }
    }
    
    free(temp);
}