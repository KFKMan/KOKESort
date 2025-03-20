#include "insertionsort.h"

void insertionSort(void *arr, size_t n, size_t elementSize, CompareFunction cmp) {
    void* temp = malloc(elementSize);
    if (!temp) 
    {
        //TODO: Throw Exception or Return Fail Status
        return;
    }

    for (size_t i = 1; i < n; i++) 
    {
        void* key = GetIndex(arr, i, elementSize);
        memcpy(temp, key, elementSize);
        size_t j = i;
        while (j > 0) 
        {
            void* prev = GetIndex(arr, j - 1, elementSize);
            if (cmp(prev, temp) <= 0)
            {
                break;
            } 
            memcpy(GetIndex(arr, j, elementSize), prev, elementSize);
            j--;
        }
        memcpy(GetIndex(arr, j, elementSize), temp, elementSize);
    }
    
    free(temp);
}