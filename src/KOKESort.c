#include <stdio.h>
#include <stdlib.h>
#include "Global.h"
#include "KOKESort.h"

void FreeMemory(void* pointer)
{
    free(pointer);
}

#ifdef USE_COMPARER
/// @brief Finding Insert Index via Binary Search
/// @return Index to Insert, -1 if error accoured
size_t FindInsertIndexBS(VariableType* arr, size_t size, VariableType element, CompareFunction comparer)
{
    //log two base n => logn Complexity

    if(!arr)
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
        if (comparer(&element, &arr[mid]) == 1) //arr[mid] < element
        #else
        if (comparer(&element, &arr[mid]) > 0)
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

VariableType* InsertToSortedArray(VariableType* arr, size_t size, VariableType element, CompareFunction comparer)
{
    if(arr == NULL || size == 0)
    {
        #ifdef DEBUG
        debugPrint("Array NULL or Array Size is Zero, returning insert element");
        #endif

        VariableType* newArr = malloc(sizeof(VariableType));  // Allocate space for one element
        if (newArr != NULL)
        {
            newArr[0] = element;
        }
        return newArr;
    }

    size_t insertIndex = FindInsertIndexBS(arr, size, element, comparer); //logN Complexity
    
    //QA: Realloc or already allocated block can be used for optimization
    VariableType* newArr = calloc(size + 1, sizeof(VariableType));

    size_t arrIndex = 0;
    size_t newArrIndex = 0;

    //QA: Don't need to set all of them, just shift from the Insert Index

    while (newArrIndex < size + 1) //N Complexity
    {
        if(arrIndex == insertIndex)
        {
            newArr[newArrIndex] = element;
            newArrIndex++;
        }

        newArr[newArrIndex] = arr[arrIndex];

        newArrIndex++;
        arrIndex++;
    }
    
    return newArr;
}

/// @brief Inserting Element to Sorted Array with BinarySearch (FindInsertIndexBS)
/// @param arr Array to Insert, null pointer can handled
/// @param currentSize Size of the Array (real element count), 0 size can handled
/// @param element Element To Insert
void InsertToSortedAllocatedArray(VariableType* arr, size_t currentSize, VariableType element, CompareFunction comparer)
{
    size_t insertIndex = FindInsertIndexBS(arr, currentSize, element, comparer); //logN Complexity
    
    size_t currentIndex = currentSize;

    while (currentIndex > insertIndex)
    {
        arr[currentIndex] = arr[currentIndex - 1];
        currentIndex--;
    }

    arr[insertIndex] = element;
}

/// @brief First Sorting Algorithm, Insertion to new array with BinarySearch 
/// | Time Complexity / O(N(N+logN)) => O(N^2) / Best O(N^2) - Average O(N^2) - Worst O(N^2) |
/// | Space Complexity / Best O(N) - Average O(N) - Worst O(N) |
/// @param arr Array to sort
/// @param size Size of array
/// @return Sorted new array
VariableType* SortV1(VariableType* arr, size_t size, CompareFunction comparer)
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
    VariableType* selfArray = calloc(size, sizeof(VariableType));

    for(unsigned int i = 0; i < size; i++) //N Complexity, 0->N
    {
        VariableType element = arr[i];
        InsertToSortedAllocatedArray(selfArray, selfArraySize, element, comparer);

        selfArraySize++;
    }

    return selfArray;
}

#endif