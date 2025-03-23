#include "KOKESortV2.h"

// Steps
// 1- Allocate and Divide Space
// 2- Create Backup Space
// 3- Start To Use

// #define LinkedListArch

void *CreatePossibilitySpace(size_t elementSize, size_t capacity)
{
    PossibilitySpace *space = AllocFn(GetPossibilitySpaceSize(capacity, elementSize));

    if (space == NULL)
    {
        return NULL;
    }

    space->Element = space + 1;
    space->Size = 0;
    space->Capacity = capacity;

    return space;
}

size_t GetPossibilitySpaceArraySize(size_t pbSpaceCount){
    return sizeof(PossibilitySpace *) * pbSpaceCount;
}

size_t GetPossibilitySpaceSize(size_t capacity, size_t elementSize)
{
    return sizeof(PossibilitySpace) + (capacity * elementSize);
}

void FreePossibilitySpaceArray(PossibilitySpace **fSpace, size_t pbSpaceCount, size_t elementSize)
{
    if (fSpace != NULL)
    {
        for (size_t i = 0; i < pbSpaceCount; i++)
        {
            PossibilitySpace *space = fSpace[i];

            DeAllocFn(space, GetPossibilitySpaceSize(space->Capacity, elementSize));
        }

        DeAllocFn(fSpace, GetPossibilitySpaceArraySize(pbSpaceCount));

        fSpace = NULL;
    }
}

PossibilitySpace **SortV2(void *array, size_t arraySize, size_t elementSize, size_t pbSpaceCount, SpaceIndexerFn indexerFn, CompareFunction comparerFn)
{
    // We need to store allocation info for deallocation.
    // TODO: Simple Comparer Support Need To Be Added
    // TODO: Same Element Support Need To Be Added

    size_t pbSpacesSize = GetPossibilitySpaceArraySize(pbSpaceCount);
    PossibilitySpace **pbSpaces = AllocFn(pbSpacesSize);

    if (pbSpaces == NULL)
    {
        perror("Allocation Error Accoured on PossibilitySpaceArray Creation");
        printf("Requested -->> " SIZE_T_IDENTIFIER "\n", pbSpacesSize);
        return NULL;
    }

    for (size_t c = 0; c < pbSpaceCount; c++)
    {
        PossibilitySpace *pbSpace = CreatePossibilitySpace(elementSize, arraySize);

        if (pbSpace == NULL)
        {
            perror("Allocation Error Accoured CreatePossibilitySpace");
            printf("Requested -->> " SIZE_T_IDENTIFIER "\n", sizeof(PossibilitySpace) + (elementSize * arraySize));

            return NULL;
        }

        pbSpaces[c] = pbSpace;
    }

    for (size_t i = 0; i < arraySize; i++)
    {
        void *currentElement = GetIndex(array, i, elementSize);
        size_t indexerIndex = indexerFn(currentElement);

        if (pbSpaceCount <= indexerIndex)
        {
            perror("Out of Possibility Space");
            return NULL;
        }

        PossibilitySpace *targetPbSpace = pbSpaces[indexerIndex];

        // Create controls, Capacity control

        void *PbArr = targetPbSpace->Element;
        void *PbArrPos = GetIndex(PbArr, targetPbSpace->Size, elementSize);
        memcpy(PbArrPos, currentElement, elementSize);
        targetPbSpace->Size++;
    }

    for (size_t i = 0; i < pbSpaceCount; i++)
    {
        PossibilitySpace *space = pbSpaces[i];
        qsort(space->Element, space->Size, elementSize, comparerFn);
    }

    return pbSpaces;
}

/// @brief 
/// @param pbSpaces 
/// @param pbSpaceCount 
/// @param arraySize 
/// @param elementSize 
/// @return Pointer allocated by AllocFn 
void *ToArray(PossibilitySpace **pbSpaces, size_t pbSpaceCount, size_t arraySize, size_t elementSize)
{
    void *arr = AllocFn(arraySize * elementSize);

    size_t index = 0;
    for (size_t i = 0; i < pbSpaceCount; i++)
    {
        PossibilitySpace *space = pbSpaces[i];

        void *arrPos = GetIndex(arr, index, elementSize);
        memcpy(arrPos, space->Element, space->Size * elementSize);

        index += space->Size;
    }

    return arr;
}
