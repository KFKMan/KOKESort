#include <stdio.h>
#include <stdlib.h>
#include "Global.h"
#include "KOKESort.h"

/// @brief Finding Insert Index via Binary Search
/// @return Index to Insert, -1 if error accoured
int FindInsertIndexBS(VariableType* arr, size_t size, VariableType element)
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

    int left = 0;
    int right = size;

    while (left < right)
    {
        int mid = left + (right - left) / 2;

        if (arr[mid] < element)
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

/// @brief Inserting Element to Sorted Array with BinarySearch (FindInsertIndexBS)
/// @param arr Array to Insert, null pointer can handled
/// @param size Size of the Array, 0 size can handled
/// @param element Element To Insert
/// @return New Array Allocated
VariableType* InsertToSortedArray(VariableType* arr, size_t size, VariableType element)
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

    int insertIndex = FindInsertIndexBS(arr, size, element); //logN Complexity
    
    //QA: Realloc or already allocated block can be used for optimization
    VariableType* newArr = calloc(size + 1, sizeof(VariableType));

    unsigned int arrIndex = 0;
    unsigned int newArrIndex = 0;

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
void InsertToSortedAllocatedArray(VariableType* arr, size_t currentSize, VariableType element)
{
    if(arr == NULL || currentSize == 0)
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

    int insertIndex = FindInsertIndexBS(arr, currentSize, element); //logN Complexity
    
    unsigned int currentIndex = currentSize;

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
VariableType* SortV1(VariableType* arr, size_t size)
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
    VariableType* selfArray = NULL;

    for(unsigned int i = 0; i < size; i++) //N Complexity, 0->N
    {
        VariableType element = arr[i];
        VariableType* tempArray = InsertToSortedArray(selfArray, selfArraySize, element); //N+logN Complexity
        
        free(selfArray);

        selfArray = tempArray;
        selfArraySize++;
    }

    return selfArray;
}

/// @brief 
/// @param tree 
/// @param element Element to Add
/// @return Return created BinaryTree element, not the given "tree" parameter 
BinaryTree* AddElementToBinaryTree(BinaryTree* tree, int element)
{
    if(tree == NULL)
    {
        return NULL;
    }

    BinaryTree* treeElement = malloc(sizeof(BinaryTree));
    treeElement->Data = element;
    treeElement->Left = NULL;
    treeElement->Right = NULL;

    //QA: Adding count property for stacking same values in BinaryTree? Or maybe denying same values?

    //QA: Skip List, Balanced Binary Tree Usage

    BinaryTree* currentTree = tree;
    while (1 == 1)
    {
        BinaryTree* tempTree;
        if(currentTree->Data < element)
        {
            tempTree = currentTree->Right;
            if(tempTree == NULL)
            {
                currentTree->Right = treeElement;
                break;
            }
        }
        else
        {
            tempTree = currentTree->Left;
            if(tempTree == NULL)
            {
                currentTree->Left = treeElement;
                break;
            }
        }

        currentTree = tempTree;
    }
    
    return treeElement;
}

BinaryTree* SortV2(VariableType* arr, size_t size)
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

    BinaryTree* tree = malloc(sizeof(BinaryTree));
    tree->Left = NULL;
    tree->Right = NULL;

    for(unsigned int i = 0; i < size; i++) //N Complexity, 0->N
    {
        VariableType element = arr[i];

        if(i == 0)
        {
            tree->Data = element;
        }
        else
        {
            AddElementToBinaryTree(tree, element);
        }
    }

    return tree;
}