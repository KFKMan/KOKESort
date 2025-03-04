#ifndef KOKESort
#define KOKESort

#define VariableType int

typedef struct BinaryTree
{ 
    VariableType Data;
    struct BinaryTree *Left;
    struct BinaryTree *Right;
} BinaryTree;

size_t FindInsertIndexBS(VariableType* arr, size_t size, VariableType element);

VariableType* InsertToSortedArray(VariableType* arr, size_t size, VariableType element);

void InsertToSortedAllocatedArray(VariableType* arr, size_t currentSize, VariableType element);

VariableType* SortV1(VariableType* arr, size_t size);

BinaryTree* SortV2(VariableType* arr, size_t size);

#endif