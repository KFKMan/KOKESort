#include <stdio.h>
#include <stdlib.h>
#include "Global.h"
#include "KOKESort.h"

void FreeMemory(void* pointer)
{
    free(pointer);
}

#ifdef USE_COMPARER

void* GetIndex(void* arr, size_t index, unsigned int elementSize)
{
    unsigned char* ptr = (unsigned char*)arr;

    void* arrIndexPtr = ptr + index * elementSize;

    return arrIndexPtr;
}

/// @brief Finding Insert Index via Binary Search
/// @return Index to Insert, -1 if error accoured
size_t FindInsertIndexBS(VariableTypeArray arr, size_t size, VariableType element, CompareFunction comparer, unsigned int elementSize)
{
    //log two base n => logn Complexity

    if(arr == NULL)
    {
        #ifdef ERROR_PRINT
        debugError("'arr' argument can't be NULL\n");
        #endif

        return -1;
    }

    if(size == 0)
    {
        return 0;
    }

    size_t left = 0;
    size_t right = size;

    while (left < right)
    {
        size_t mid = left + (right - left) / 2;

        #ifdef COMPARE_USE_SIMPLE
        if (comparer(element, GetIndex(arr, mid, elementSize)) == 1) //arr[mid] < element
        #else
        if (comparer(element, GetIndex(arr, mid, elementSize)) > 0)
        #endif
        {
            left = mid + 1;
        }
        else
        {
            right = mid;
        }
    }

    return left;
}

VariableTypeArray InsertToSortedArray(VariableTypeArray arr, size_t size, VariableType element, CompareFunction comparer, unsigned int elementSize)
{
    if(arr == NULL || size == 0)
    {
        #ifdef DEBUG
        debugPrint("Array NULL or Array Size is Zero, returning insert element");
        #endif

        VariableTypeArray newArr = malloc(elementSize);  // Allocate space for one element
        if (newArr == NULL)
        {
            return NULL;
        }
        memcpy(newArr, element, elementSize);
        return newArr;
    }

    size_t insertIndex = FindInsertIndexBS(arr, size, element, comparer, elementSize); //logN Complexity
    
    VariableTypeArray newArr = calloc(size + 1, elementSize);

    size_t arrIndex = 0;
    size_t newArrIndex = 0;

    while (newArrIndex < size + 1) //N Complexity
    {
        if(arrIndex == insertIndex)
        {
            void* newArrInsertIndexPtr = GetIndex(newArr, newArrIndex, elementSize);
            memcpy(newArrInsertIndexPtr, element, elementSize);
            newArrIndex++;
        }

        void* newArrIndexPtr = GetIndex(newArr, newArrIndex, elementSize);
        memcpy(newArrIndexPtr, GetIndex(arr, arrIndex, elementSize), elementSize);

        newArrIndex++;
        arrIndex++;
    }
    
    return newArr;
}

/// @brief Inserting Element to Sorted Array with BinarySearch (FindInsertIndexBS)
/// @param arr Array to Insert, null pointer can handled
/// @param currentSize Size of the Array (real element count), 0 size can handled
/// @param element Element To Insert
void InsertToSortedAllocatedArray(VariableTypeArray arr, size_t currentSize, VariableType element, CompareFunction comparer, unsigned int elementSize)
{
    size_t insertIndex = FindInsertIndexBS(arr, currentSize, element, comparer, elementSize); //logN Complexity
    
    size_t currentIndex = currentSize;

    while (currentIndex > insertIndex)
    {
        void* arrCurrentIndexPtr = GetIndex(arr, currentIndex, elementSize);
        void* arrPreviousIndexPtr = GetIndex(arr, currentIndex - 1, elementSize);

        memcpy(arrCurrentIndexPtr, arrPreviousIndexPtr, elementSize);

        currentIndex--;
    }

    void* arrInsertIndexPtr = GetIndex(arr, insertIndex, elementSize);
    memcpy(arrInsertIndexPtr, element, elementSize);
}

/// @brief First Sorting Algorithm, Insertion to new array with BinarySearch 
/// | Time Complexity / O(N(N+logN)) => O(N^2) / Best O(N^2) - Average O(N^2) - Worst O(N^2) |
/// | Space Complexity / Best O(N) - Average O(N) - Worst O(N) |
/// @param arr Array to sort
/// @param size Size of array
/// @return Sorted new array
VariableTypeArray SortV1(VariableTypeArray arr, size_t size, CompareFunction comparer, unsigned int elementSize)
{
    //Argument checks
    if (!arr)
    {
        #ifdef ERROR_PRINT
        debugError("'arr' argument can't be NULL\n");
        #endif

        return NULL;
    }

    if(size == 0)
    {
        #ifdef ERROR_PRINT
        debugError("'size' argument can't be zero\n");
        #endif
        
        return NULL;
    }

    size_t selfArraySize = 0;
    VariableType* selfArray = calloc(size, elementSize);

    for(unsigned int i = 0; i < size; i++) //N Complexity, 0->N
    {
        void* element = GetIndex(arr, i, elementSize);

        InsertToSortedAllocatedArray(selfArray, selfArraySize, element, comparer, elementSize);

        selfArraySize++;
    }

    return selfArray;
}

#endif