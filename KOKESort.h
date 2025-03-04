#ifndef KOKESort
#define KOKESort

#define VariableType int

typedef struct BinaryTree
{ 
    VariableType Data;
    struct node *Left;
    struct node *Right;
} BinaryTree;

int FindInsertIndexBS(VariableType* arr, size_t size, VariableType element);

VariableType* InsertToSortedArray(VariableType* arr, size_t size, VariableType element);

VariableType* SortV1(VariableType* arr, size_t size);

BinaryTree* SortV2(VariableType* arr, size_t size);

#endif