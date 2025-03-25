#ifndef KOKESort
#define KOKESort

#include <string.h> //For memcpy
#include "Global.h"

#ifdef __cplusplus
 extern "C" {  /* C++ name mangling */
 #endif

//Enabling Simple Compare Functions -1 (less), 0 (equal), 1 (more)
//#define COMPARE_USE_SIMPLE 1


#ifdef USE_COMPARER

#define VariableType void*
#define VariableTypeArray VariableType

/// @brief Finding Insert Index on Sorted Array via Binary Search
/// @param arr Sorted Array, CAN'T handle NULL array
/// @param size Filled size of the array
/// @param element Element to find insert index
/// @param comparer Comparer Function
/// @param elementSize Size of the one element.
/// @return Insert Index
KOKESORT_API size_t FindInsertIndexBSNonSafe(VariableTypeArray arr, size_t size, VariableType element, CompareFunction comparer, unsigned int elementSize);

/// @brief Finding Insert Index on Sorted Array via Binary Search
/// @param arr Sorted Array
/// @param size Filled size of the array
/// @param element Element to find insert index
/// @param comparer Comparer Function
/// @param elementSize Size of the one element.
/// @return Insert Index, If array null returning 0 (this doesn't mean all 0 return values is error, for example if the element is less than other array element values it's must be inserted to start of the array so algorithm will return 0)
KOKESORT_API size_t FindInsertIndexBS(VariableTypeArray arr, size_t size, VariableType element, CompareFunction comparer, unsigned int elementSize);

/// @brief Inserting Element to Copy of Sorted Array with BinarySearch \ref FindInsertIndexBS
/// @param arr Sorted Array, CAN'T handle NULL array
/// @param size Filled size of the array, CAN'T handle ZERO size
/// @param element Element to Insert
/// @param comparer Comparer Function
/// @param elementSize Size of the one element.
/// @return New allocated array or /ref NULL if error accoured.
KOKESORT_API VariableTypeArray InsertToSortedArrayNonSafe(VariableTypeArray arr, size_t size, VariableType element, CompareFunction comparer, unsigned int elementSize);

/// @brief Inserting Element to Copy of Sorted Array with BinarySearch \ref FindInsertIndexBS
/// @param arr Sorted Array
/// @param size Filled size of the array
/// @param element Element to Insert
/// @param comparer Comparer Function
/// @param elementSize Size of the one element.
/// @return New allocated array or /ref NULL if error accoured.
KOKESORT_API VariableTypeArray InsertToSortedArray(VariableTypeArray arr, size_t size, VariableType element, CompareFunction comparer, unsigned int elementSize);

/// @brief Inserting Element to Sorted Array with BinarySearch \ref FindInsertIndexBS
/// @param arr Sorted Array (Allocated means the array have space in the last), CAN'T handle NULL array
/// @param currentSize Filled size of the array, array must be bigger than that, CAN'T handle ZERO size
/// @param element Element to Insert
/// @param comparer Comparer Function
/// @param elementSize Size of the one element.
/// @return Return nothing
KOKESORT_API void InsertToSortedAllocatedArrayNonSafe(VariableTypeArray arr, size_t currentSize, VariableType element, CompareFunction comparer, unsigned int elementSize);


/// @brief Inserting Element to Sorted Array with BinarySearch \ref FindInsertIndexBS
/// @param arr Sorted Array (Allocated means the array have space in the last)
/// @param currentSize Filled size of the array, array must be bigger than that
/// @param element Element to Insert
/// @param comparer Comparer Function
/// @param elementSize Size of the one element.
/// @return \ref SUCCESS if successed, \ref FAIL if failed.
KOKESORT_API int InsertToSortedAllocatedArray(VariableTypeArray arr, size_t currentSize, VariableType element, CompareFunction comparer, unsigned int elementSize);

KOKESORT_API void SortV1SelfAllocatedNonZeroNonSafe(VariableTypeArray unsortedArr, size_t size, CompareFunction comparer, unsigned int elementSize);

KOKESORT_API void SortV1AllocatedNonZeroNonSafe(VariableTypeArray unsortedArr, VariableTypeArray allocatedArr, size_t size, CompareFunction comparer, unsigned int elementSize);

KOKESORT_API void SortV1AllocatedNonSafe(VariableTypeArray unsortedArr, VariableTypeArray allocatedArr, size_t size, CompareFunction comparer, unsigned int elementSize);

KOKESORT_API VariableTypeArray SortV1NonSafe(VariableTypeArray arr, size_t size, CompareFunction comparer, unsigned int elementSize);

KOKESORT_API VariableTypeArray SortV1(VariableTypeArray arr, size_t size, unsigned int elementSize, CompareFunction comparer);

KOKESORT_API void SortV1SelfNonSafe(VariableTypeArray array, size_t size, CompareFunction comparer, unsigned int elementSize);

KOKESORT_API int SortV1Self(VariableTypeArray array, size_t size, unsigned int elementSize, CompareFunction comparer);

#endif

#ifdef __cplusplus
 };
#endif

#endif