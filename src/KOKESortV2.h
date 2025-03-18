#ifndef KOKESortV2
#define KOKESortV2

#include <Global.h>
#include <string.h> //For memcpy

#ifdef __cplusplus
extern "C" {  /* C++ name mangling */
#endif

typedef size_t (*SpaceIndexerFn)(const void *);

typedef struct PossibilitySpace
{
    size_t Size;
    size_t Capacity;
    void *Element;

} PossibilitySpace;

KOKESORT_API void* CreatePossibilitySpace(size_t elementSize, size_t capacity);

KOKESORT_API PossibilitySpace** SortV2(void *array, size_t arraySize, size_t elementSize, size_t pbSpaceCount, SpaceIndexerFn indexerFn, CompareFunction comparerFn);

KOKESORT_API void* ToArray(PossibilitySpace** pbSpaces, size_t pbSpaceCount, size_t arraySize, size_t elementSize);

#ifdef __cplusplus
};
#endif

#endif