#include "mergesort.h"

void merge(void *arr, size_t left, size_t mid, size_t right, size_t elementSize, CompareFunction cmp) {
    size_t leftSize = mid - left + 1;
    size_t rightSize = right - mid;

    void* leftArr = malloc(leftSize * elementSize);
    if (!leftArr)
    {
        return;
    }

    void* rightArr = malloc(rightSize * elementSize);
    if(!rightArr)
    {
        free(leftArr);
        return;
    }

    memcpy(leftArr, (char*)arr + left * elementSize, leftSize * elementSize);
    memcpy(rightArr, (char*)arr + (mid + 1) * elementSize, rightSize * elementSize);

    size_t i = 0, j = 0, k = left;
    while (i < leftSize && j < rightSize) 
    {
        if (cmp((char*)leftArr + i * elementSize, (char*)rightArr + j * elementSize) <= 0) 
        {
            memcpy((char*)arr + k * elementSize, (char*)leftArr + i * elementSize, elementSize);
            i++;
        } 
        else 
        {
            memcpy((char*)arr + k * elementSize, (char*)rightArr + j * elementSize, elementSize);
            j++;
        }
        k++;
    }

    while (i < leftSize) 
    {
        memcpy((char*)arr + k * elementSize, (char*)leftArr + i * elementSize, elementSize);
        i++, k++;
    }

    while (j < rightSize) 
    {
        memcpy((char*)arr + k * elementSize, (char*)rightArr + j * elementSize, elementSize);
        j++, k++;
    }

    free(leftArr);
    free(rightArr);
}

void mergeSortOpt(void *arr, size_t left, size_t right, size_t elementSize, CompareFunction cmp) {
    if (left < right) 
    {
        size_t mid = left + (right - left) / 2;
        mergeSortOpt(arr, left, mid, elementSize, cmp);
        mergeSortOpt(arr, mid + 1, right, elementSize, cmp);
        merge(arr, left, mid, right, elementSize, cmp);
    }
}

void mergeSort(void *arr, size_t n, size_t elementSize, CompareFunction cmp)
{
    mergeSortOpt(arr, 0, n-1, elementSize, cmp);
}