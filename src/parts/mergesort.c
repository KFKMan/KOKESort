#include "mergesort.h"

void merge(void *arr, size_t left, size_t mid, size_t right, size_t elementSize, CompareFunction cmp) {
    size_t leftSize = mid - left + 1;
    size_t rightSize = right - mid;
    void* leftArr = malloc(leftSize * elementSize);
    void* rightArr = malloc(rightSize * elementSize);
    if (!leftArr || !rightArr)
    {
        return;
    }

    memcpy(leftArr, GetIndex(arr, left, elementSize), leftSize * elementSize);
    memcpy(rightArr, GetIndex(arr, mid + 1, elementSize), rightSize * elementSize);

    size_t i = 0, j = 0, k = left;
    while (i < leftSize && j < rightSize) 
    {
        if (cmp(GetIndex(leftArr, i, elementSize), GetIndex(rightArr, j, elementSize)) <= 0) 
        {
            memcpy(GetIndex(arr, k, elementSize), GetIndex(leftArr, i, elementSize), elementSize);
            i++;
        } 
        else 
        {
            memcpy(GetIndex(arr, k, elementSize), GetIndex(rightArr, j, elementSize), elementSize);
            j++;
        }
        k++;
    }

    while (i < leftSize) 
    {
        memcpy(GetIndex(arr, k, elementSize), GetIndex(leftArr, i, elementSize), elementSize);
        i++, k++;
    }

    while (j < rightSize) 
    {
        memcpy(GetIndex(arr, k, elementSize), GetIndex(rightArr, j, elementSize), elementSize);
        j++, k++;
    }

    free(leftArr);
    free(rightArr);
}

/// @brief Sorts an array using Merge Sort
void mergeSort(void *arr, size_t left, size_t right, size_t elementSize, CompareFunction cmp) {
    if (left < right) 
    {
        size_t mid = left + (right - left) / 2;
        mergeSort(arr, left, mid, elementSize, cmp);
        mergeSort(arr, mid + 1, right, elementSize, cmp);
        merge(arr, left, mid, right, elementSize, cmp);
    }
}