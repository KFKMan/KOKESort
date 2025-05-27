#ifndef KOKESortV2
#define KOKESortV2

#include <Global.h>
#include <string.h> //For memcpy
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {  /* C++ name mangling */
#endif

typedef size_t (*SpaceIndexerFn)(const void *, const void*);

typedef struct PossibilitySpace
{
    size_t Size;
    size_t Capacity;
    void *Element;

} PossibilitySpace;

KOKESORT_API PossibilitySpace *CreatePossibilitySpace();
KOKESORT_API PossibilitySpace* CreatePossibilitySpaceArray(size_t pbSpaceCount);

KOKESORT_API bool AllocatePossibilitySpaceElements(
    size_t elementSize,
    size_t pbSpaceCount,
    PossibilitySpace* pbSpacesStoreArray
);

KOKESORT_API void DeAllocatePossibilitySpaceElements(PossibilitySpace* pbSpace, size_t elementSize);

KOKESORT_API void FreePossibilitySpace(PossibilitySpace* pbSpace, size_t elementSize);

KOKESORT_API void FreePossibilitySpaceArray(PossibilitySpace* pbSpaceArray, size_t pbSpaceCount, size_t elementSize);

KOKESORT_API PossibilitySpace* SortV2(void *array, size_t arraySize, size_t elementSize, size_t pbSpaceCount, SpaceIndexerFn indexerFn, void* indexerFnData, CompareFunction comparerFn, SortFunction sortFn);

KOKESORT_API bool CountPossibilitySpaces(
    void *array,
    size_t arraySize,
    size_t elementSize,
    size_t pbSpaceCount,
    SpaceIndexerFn indexerFn,
    void* indexerFnData,
    PossibilitySpace* pbSpacesStoreArray
);

KOKESORT_API bool CopyElementsToPossibilitySpaces(
    void *array,
    size_t arraySize,
    size_t elementSize,
    size_t pbSpaceCount,
    PossibilitySpace* pbSpacesStoreArray,
    SpaceIndexerFn indexerFn,
    void* indexerFnData
);

KOKESORT_API bool SortPossibilitySpaces(
    size_t pbSpaceCount,
    PossibilitySpace* pbSpacesStoreArray,
    size_t elementSize,
    CompareFunction comparerFn,
    SortFunction sortFn
);

KOKESORT_API void* ToArray(PossibilitySpace* pbSpaces, size_t pbSpaceCount, size_t arraySize, size_t elementSize);

#ifdef __cplusplus
};
#endif

#endif