#include "bubblesort.h"

void bubbleSort(void *arr, size_t n, size_t elementSize, CompareFunction cmp) 
{
    void* temp = malloc(elementSize);
    if (!temp)
    {
        //TODO: Throw Exception or Return Fail Status
        return;
    }

    for (size_t i = 0; i < n - 1; i++) 
    {
        for (size_t j = 0; j < n - i - 1; j++) 
        {
            void* a = GetIndex(arr, j, elementSize);
            void* b = GetIndex(arr, j + 1, elementSize);
            if (cmp(a, b) > 0) 
            {
                memcpy(temp, a, elementSize);
                memcpy(a, b, elementSize);
                memcpy(b, temp, elementSize);
            }
        }
    }

    free(temp);
}