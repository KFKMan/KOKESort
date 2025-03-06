#ifdef NO_INCLUDE_DIR
#include "../../Global.h"
#include "../../Config.h"
#else
#include "Global.h"
#include "Config.h"
#endif

extern "C"
{
    typedef struct BinaryTree
    {    
        VariableType Data;
        struct BinaryTree *Left;
        struct BinaryTree *Right;
    } BinaryTree;

    KOKESORT_API size_t FindInsertIndexBS(VariableType* arr, size_t size, VariableType element);

    KOKESORT_API VariableType* InsertToSortedArray(VariableType* arr, size_t size, VariableType element);

    KOKESORT_API void InsertToSortedAllocatedArray(VariableType* arr, size_t currentSize, VariableType element);

    KOKESORT_API VariableType* SortV1(VariableType* arr, size_t size);

    KOKESORT_API BinaryTree* SortV2(VariableType* arr, size_t size);

    KOKESORT_API void FreeMemory(void* pointer);
}