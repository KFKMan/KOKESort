#include "KOKESort.h"

#ifdef USE_COMPARER

size_t FindInsertIndexBSNonSafe(VariableTypeArray arr, size_t size, VariableType element, CompareFunction comparer, unsigned int elementSize)
{
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

/// @brief Finding Insert Index via Binary Search
size_t FindInsertIndexBS(VariableTypeArray arr, size_t size, VariableType element, CompareFunction comparer, unsigned int elementSize)
{
    //log two base n => logn Complexity

    if(!arr || size == 0)
    {
        return 0;
    }

    return FindInsertIndexBSNonSafe(arr, size, element, comparer, elementSize);
}

VariableTypeArray InsertToSortedArrayNonSafe(VariableTypeArray arr, size_t size, VariableType element, CompareFunction comparer, unsigned int elementSize)
{
    size_t insertIndex = FindInsertIndexBSNonSafe(arr, size, element, comparer, elementSize);
    
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

VariableTypeArray InsertToSortedArray(VariableTypeArray arr, size_t size, VariableType element, CompareFunction comparer, unsigned int elementSize)
{
    if(!arr || size == 0)
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

    return InsertToSortedArrayNonSafe(arr, size, element, comparer, elementSize);
}

void MoveElement(VariableTypeArray arr, size_t elementSize, int fromIndex, int toIndex)
{
    void* elementBackup = malloc(elementSize);
    memcpy(elementBackup, GetIndex(arr, fromIndex, elementSize), elementSize);

    memmove(GetIndex(arr, toIndex + 1, elementSize), 
                GetIndex(arr, toIndex, elementSize), 
                (fromIndex - toIndex) * elementSize);

    //In our senario fromIndex always bigger than toIndex
    /*
    if (fromIndex > toIndex) 
    {
        memmove(GetIndex(arr, toIndex + 1, elementSize), 
                GetIndex(arr, toIndex, elementSize), 
                (fromIndex - toIndex) * elementSize);
    } 
    else 
    {
        memmove(GetIndex(arr, toIndex, elementSize), 
                GetIndex(arr, toIndex + 1, elementSize), 
                (fromIndex - toIndex) * elementSize);
    }
    */

    memcpy(GetIndex(arr, toIndex, elementSize), elementBackup, elementSize);
    free(elementBackup);
}

void InsertToSortedAllocatedArrayNonSafe(VariableTypeArray arr, size_t currentSize, VariableType element, CompareFunction comparer, unsigned int elementSize)
{
    size_t insertIndex = FindInsertIndexBSNonSafe(arr, currentSize, element, comparer, elementSize);
    
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

/// @brief Inserting Element to Sorted Array with BinarySearch (FindInsertIndexBS)
/// @param arr Array to Insert, null pointer can handled
/// @param currentSize Size of the Array (real element count), 0 size can handled
/// @param element Element To Insert
/// @return 1 if success, 0 if fail
int InsertToSortedAllocatedArray(VariableTypeArray arr, size_t currentSize, VariableType element, CompareFunction comparer, unsigned int elementSize)
{
    if(!arr)
    {
        return 0;
    }

    if(currentSize == 0)
    {
        memcpy(arr, element, elementSize);
        return 1;
    }

    InsertToSortedAllocatedArrayNonSafe(arr, currentSize, element, comparer, elementSize);
    return 1;
}

void SortV1AllocatedNonZeroNonSafe(VariableTypeArray unsortedArr, VariableTypeArray allocatedArr, size_t size, CompareFunction comparer, unsigned int elementSize)
{
    size_t selfArraySize = 1;

    for(unsigned int i = 1; i < size; i++) //N Complexity, 0->N
    {
        void* element = GetIndex(unsortedArr, i, elementSize);

        InsertToSortedAllocatedArrayNonSafe(allocatedArr, selfArraySize, element, comparer, elementSize);

        selfArraySize++;
    }
}

void SortV1SelfAllocatedNonZeroNonSafe(VariableTypeArray unsortedArr, size_t size, CompareFunction comparer, unsigned int elementSize)
{
    size_t selfArraySize = 1;
    //void* elementBackup = malloc(elementSize);

    for(unsigned int i = 1; i < size; i++) //N Complexity, 0->N
    {
        void* element = GetIndex(unsortedArr, i, elementSize);
        //memcpy(elementBackup, element, elementSize);

        size_t insertIndex = FindInsertIndexBSNonSafe(unsortedArr, i, element, comparer, elementSize);

        MoveElement(unsortedArr, elementSize, i, insertIndex);

        selfArraySize++;
    }
}

void SortV1AllocatedNonSafe(VariableTypeArray unsortedArr, VariableTypeArray allocatedArr, size_t size, CompareFunction comparer, unsigned int elementSize)
{
    memcpy(allocatedArr, unsortedArr, elementSize);

    SortV1AllocatedNonZeroNonSafe(unsortedArr, allocatedArr, size, comparer, elementSize);
}

VariableTypeArray SortV1NonSafe(VariableTypeArray arr, size_t size, CompareFunction comparer, unsigned int elementSize)
{
    VariableType* selfArray = malloc(size * elementSize);

    if(!selfArray)
    {
        return NULL;
    }

    SortV1AllocatedNonSafe(arr, selfArray, size, comparer, elementSize);

    return selfArray;
}

/// @brief First Sorting Algorithm, Insertion to new array with BinarySearch 
/// | Time Complexity / O(N(N+logN)) => O(N^2) / Best O(N^2) - Average O(N^2) - Worst O(N^2) |
/// | Space Complexity / Best O(N) - Average O(N) - Worst O(N) |
/// @param arr Array to sort
/// @param size Size of array
/// @return Sorted new array or NULL if array is not valid or size is zero
VariableTypeArray SortV1(VariableTypeArray arr, size_t size, CompareFunction comparer, unsigned int elementSize)
{
    //Argument checks
    if (!arr || size == 0)
    {
        return NULL;
    }

    return SortV1NonSafe(arr, size, comparer, elementSize);
}

void SortV1SelfNonSafe(VariableTypeArray array, size_t size, CompareFunction comparer, unsigned int elementSize)
{
    SortV1SelfAllocatedNonZeroNonSafe(array, size, comparer, elementSize);
}

/// @brief 
/// @param array 
/// @param size 
/// @param comparer 
/// @param elementSize 
/// @return Success or not
int SortV1Self(VariableTypeArray array, size_t size, CompareFunction comparer, unsigned int elementSize)
{
    if(!array || size == 0)
    {
        return FAIL;
    }

    SortV1SelfNonSafe(array, size, comparer, elementSize);

    return SUCCESS;
}

#endif