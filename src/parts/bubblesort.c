#include "bubblesort.h"

void bubbleSort(void *arr, size_t n, size_t elementSize, CompareFunction cmp) 
{
    for (size_t i = 0; i < n - 1; i++) 
    {
        int stop = SUCCESS;
        for (size_t j = 0; j < n - i - 1; j++) 
        {
            void* a = GetIndex(arr, j, elementSize);
            void* b = GetIndex(arr, j + 1, elementSize);
            if (cmp(a, b) > 0) 
            {
                swap(a, b, elementSize);
                stop = FAIL;
            }
        }
        if(stop == SUCCESS)
        {
            break;
        }
    }
}