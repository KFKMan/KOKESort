#include <stdio.h>
#include <stdlib.h>

//O(n) => Normal Arrayda ekleme yapma
//O(1) => Linked Listte ekleme yapma
//O(log2n) => Linked Listte bir değere erişme

//O(n)

struct node
{ 
    int data;
    struct node *left;
    struct node *right;
};


LinkedListInt* IntArrayToLinkedList(int* arr, size_t size) //O(n)
{
    //Argument checks
    if (!arr)
    {
        #ifdef ERROR_PRINT
        fprintf(stderr, "'arr' argument can't be NULL\n");
        #endif

        return NULL;
    }

    if(size == 0)
    {
        #ifdef ERROR_PRINT
        fprintf(stderr, "'size' argument can't be zero\n");
        #endif
        
        return NULL;
    }

    //Allocate all nodes in one
    LinkedListInt* nodes = (LinkedListInt*)calloc(size, sizeof(LinkedListInt));
    if (!nodes) {
        #ifdef ERROR_PRINT
        fprintf(stderr, "Memory Allocation Error\n");
        #endif

        return NULL;
    }

    //TODO: Can be more optimized
    for (size_t i = 0; i < size; i++) {
        nodes[i].Value = arr[i];

        if (i > 0) 
        {
            nodes[i].Previous = &nodes[i - 1];
            nodes[i - 1].Next = &nodes[i];
        }
    }

    //Returning first element
    return &nodes[0]; 
}


/// @brief Function symbol for testing
/// @return Always return 1
int TestSymbol(){
    return 1;
}

/// @brief Sorting array with KOKE Sorting Algorithm
/// @param arr Array to sort
/// @param size Item count of the "arr"
/// @return NULL pointer if error accoured 
void Sort(LinkedListInt* arr, size_t size){
    for (size_t i = 0; i < size; i++) { //N
        #ifdef DEBUG
        printf("%d ", GetIndex(arr, 0, i));
        #endif
    
        GetInsertIndex(arr, size, GetIndex(arr,0,i));
    }
}

void AddItemToSortedArray(int** arr, size_t size, int value){

}

/// @brief Getting insert index in the sorted array with Binary Search
/// @param arr Array
/// @param size Item count of the "arr"
/// @param value Value to calculate insert index
/// @return Insert Index
LinkedListInt* GetInsertIndex(LinkedListInt* arr, size_t size, int value){ //Worst O(lognlognlogn) | Best O(lognlogn)
    LinkedListInt* currentNode = arr;
    size_t currentIndex = 0;
    

    size_t left = 0, right = size;
    
    while (left < right) {
        size_t mid = left + ((right - left) / 2);

        currentNode = GetIndex(currentNode, currentIndex, mid);
        currentIndex = mid;

        if (*currentNode->Value == value) 
        {
            return mid;  // Value Found
        } else if (*currentNode->Value < value) 
        {
            left = mid + 1;  // Go to right
        } else 
        {
            right = mid;  // Go to left
        }
    }

    currentNode = GetIndex(currentNode, currentIndex, left);
    currentIndex = left;
    return left;
}


/// @brief 
/// @param node 
/// @param currentIndex 
/// @param index 
/// @return NULL if any node when going to target index is NULL 
LinkedListInt* GetIndex(LinkedListInt* node, size_t currentIndex,size_t index) //O(logn)
{
    LinkedListInt* crrNode = node;

    size_t nodeIndex = currentIndex;

    if (index > nodeIndex) 
    {
        // İleri gitme
        while (crrNode != NULL && nodeIndex < index) 
        {
            crrNode = crrNode->Next;
            nodeIndex++;
        }
    } 
    else if (index < nodeIndex) 
    {
        while (crrNode != NULL && nodeIndex > index) 
        {
            crrNode = crrNode->Previous;
            nodeIndex--;
        }
    }

    return (index == nodeIndex) ? crrNode : NULL;
}