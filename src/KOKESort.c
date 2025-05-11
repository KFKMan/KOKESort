#include "KOKESort.h"

#ifdef USE_COMPARER

size_t FindInsertIndexBSNonSafe(VariableTypeArray arr, size_t size, VariableType element, CompareFunction comparer, size_t elementSize)
{
    size_t left = 0;
    size_t right = size;

    while (left < right)
    {
        size_t mid = left + (right - left) / 2;

        //printf("Left %Iu Right %Iu Mid (Accessing) %Iu \n", left, right, mid);

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
size_t FindInsertIndexBS(VariableTypeArray arr, size_t size, VariableType element, CompareFunction comparer, size_t elementSize)
{
    //log two base n => logn Complexity

    if(!arr)
    {
        return 0;
    }

    return FindInsertIndexBSNonSafe(arr, size, element, comparer, elementSize);
}

VariableTypeArray InsertToSortedArrayNonSafe(VariableTypeArray arr, size_t size, VariableType element, CompareFunction comparer, size_t elementSize)
{
    size_t insertIndex = FindInsertIndexBSNonSafe(arr, size, element, comparer, elementSize);
    
    VariableTypeArray newArr = malloc((size + 1) * elementSize);

    if(newArr == NULL)
    {
        return NULL;
    }

    size_t arrIndex = 0;
    size_t newArrIndex = 0;

    while (newArrIndex < size + 1) //N Complexity
    {
        if(arrIndex == insertIndex)
        {
            void* newArrInsertIndexPtr = GetIndex(newArr, newArrIndex, elementSize);
            void* newArrInsertIndexCpyRes = memcpy(newArrInsertIndexPtr, element, elementSize);

            if(newArrInsertIndexCpyRes == NULL)
            {
                return NULL;
            }

            newArrIndex++;
        }

        void* newArrIndexPtr = GetIndex(newArr, newArrIndex, elementSize);
        void* newArrIndexCpyRes = memcpy(newArrIndexPtr, GetIndex(arr, arrIndex, elementSize), elementSize);

        if(newArrIndexCpyRes == NULL)
        {
            return NULL;
        }

        newArrIndex++;
        arrIndex++;
    }
    
    return newArr;
}

VariableTypeArray InsertToSortedArray(VariableTypeArray arr, size_t size, VariableType element, CompareFunction comparer, size_t elementSize)
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

int MoveElement(VariableTypeArray arr, size_t elementSize, int fromIndex, int toIndex)
{
    void* elementBackup = malloc(elementSize);

    if(elementBackup == NULL)
    {
        perror("MoveElement elementBackup can't allocated");
        return FAIL;
    }
    
    memcpy(elementBackup, GetIndex(arr, fromIndex, elementSize), elementSize);

    memcpy(GetIndex(arr, toIndex + 1, elementSize), 
           GetIndex(arr, toIndex, elementSize), 
           (fromIndex - toIndex) * elementSize);

    memcpy(GetIndex(arr, toIndex, elementSize), elementBackup, elementSize);

    free(elementBackup);

    return SUCCESS;
}

void InsertToSortedAllocatedArrayNonSafe(VariableTypeArray arr, size_t currentSize, VariableType element, CompareFunction comparer, size_t elementSize)
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

int InsertToSortedAllocatedArray(VariableTypeArray arr, size_t currentSize, VariableType element, CompareFunction comparer, size_t elementSize)
{
    if(!arr)
    {
        return 0;
    }

    if(currentSize == 0)
    {
        void* copyRes = memcpy(arr, element, elementSize);
        if(copyRes == NULL)
        {
            return 0;
        }
        return 1;
    }

    InsertToSortedAllocatedArrayNonSafe(arr, currentSize, element, comparer, elementSize);
    return 1;
}

void SortV1AllocatedNonZeroNonSafe(VariableTypeArray unsortedArr, VariableTypeArray allocatedArr, size_t size, CompareFunction comparer, size_t elementSize)
{
    size_t selfArraySize = 1;

    for(unsigned int i = 1; i < size; i++) //N Complexity, 0->N
    {
        void* element = GetIndex(unsortedArr, i, elementSize);

        InsertToSortedAllocatedArrayNonSafe(allocatedArr, selfArraySize, element, comparer, elementSize);

        selfArraySize++;
    }
}

void SortV1SelfAllocatedNonZeroNonSafe(VariableTypeArray unsortedArr, size_t size, CompareFunction comparer, size_t elementSize)
{
    //void* elementBackup = malloc(elementSize);

    //printf("SortV1 Self Called \n");
    //printf("Pointer Size %Iu \n", sizeof(void*));

    for(unsigned int i = 1; i < size; i++) //N Complexity, 0->N
    {
        void* element = GetIndex(unsortedArr, i, elementSize);
        //memcpy(elementBackup, element, elementSize);
        //printf("Element Pointer %p \n", element);

        //printf("Finding Insert Index \n");
        size_t insertIndex = FindInsertIndexBSNonSafe(unsortedArr, i, element, comparer, elementSize);

        //printf("Moving Element \n");
        MoveElement(unsortedArr, elementSize, i, insertIndex);
    }
}

void SortV1AllocatedNonSafe(VariableTypeArray unsortedArr, VariableTypeArray allocatedArr, size_t size, CompareFunction comparer, size_t elementSize)
{
    memcpy(allocatedArr, unsortedArr, elementSize);

    SortV1AllocatedNonZeroNonSafe(unsortedArr, allocatedArr, size, comparer, elementSize);
}

VariableTypeArray SortV1NonSafe(VariableTypeArray arr, size_t size, CompareFunction comparer, size_t elementSize)
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
VariableTypeArray SortV1(VariableTypeArray arr, size_t size, size_t elementSize, CompareFunction comparer)
{
    //Argument checks
    if (!arr || size == 0)
    {
        return NULL;
    }

    return SortV1NonSafe(arr, size, comparer, elementSize);
}

void SortV1SelfNonSafe(VariableTypeArray array, size_t size, CompareFunction comparer, size_t elementSize)
{
    SortV1SelfAllocatedNonZeroNonSafe(array, size, comparer, elementSize);
}

/// @brief 
/// @param array 
/// @param size 
/// @param comparer 
/// @param elementSize 
/// @return Success or not
void SortV1Self(VariableTypeArray array, size_t size, size_t elementSize, CompareFunction comparer)
{
    if(!array || size == 0)
    {
        return;
    }

    SortV1SelfNonSafe(array, size, comparer, elementSize);
}

#endif