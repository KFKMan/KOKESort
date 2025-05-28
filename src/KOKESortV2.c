#include "KOKESortV2.h"

// Steps
// 1- Allocate and Divide Space
// 2- Create Backup Space
// 3- Start To Use

// #define LinkedListArch

PossibilitySpace* CreatePossibilitySpaceArray(size_t pbSpaceCount)
{
    if(pbSpaceCount == 0)
    {
        K_LOG_ERROR("Parameter 'pbSpaceCount' is zero.");
        return NULL;
    }

    PossibilitySpace* pbSpaces = AllocFn(pbSpaceCount * sizeof(PossibilitySpace));
    if (pbSpaces == NULL)
    {
        perror("Failed to allocate PossibilitySpace array");
        return NULL;
    }

    for (size_t i = 0; i < pbSpaceCount; ++i)
    {
        pbSpaces[i].Size = 0;
        pbSpaces[i].Capacity = 0;
        pbSpaces[i].Element = NULL;
    }

    return pbSpaces;
}

bool AllocatePossibilitySpaceElements(
    size_t elementSize,
    size_t pbSpaceCount,
    PossibilitySpace* pbSpacesStoreArray
)
{
    if(elementSize == 0)
    {
        K_LOG_ERROR("Parameter 'elementSize' is zero.");
        return false;
    }

    if(pbSpaceCount == 0)
    {
        K_LOG_ERROR("Parameter 'pbSpaceCount' is zero.");
        return false;
    }

    if(!pbSpacesStoreArray)
    {
        K_LOG_ERROR("Parameter 'pbSpacesStoreArray' is NULL.");
        return false;
    }

    for (size_t i = 0; i < pbSpaceCount; ++i)
    {
        size_t capacity = pbSpacesStoreArray[i].Capacity;

        pbSpacesStoreArray[i].Size = 0;

        if (capacity == 0)
        {
            pbSpacesStoreArray[i].Element = NULL;
            continue;
        }

        pbSpacesStoreArray[i].Element = AllocFn(capacity * elementSize);

        if (pbSpacesStoreArray[i].Element == NULL)
        {
            perror("Allocation failed");

            for (size_t j = 0; j < i; ++j)
            {
                DeAllocatePossibilitySpaceElements(&pbSpacesStoreArray[j], elementSize);
            }
            return false;
        }
    }

    return true;
}

void DeAllocatePossibilitySpaceElements(PossibilitySpace* pbSpace, size_t elementSize)
{
    if(pbSpace == NULL)
    {
        K_LOG_ERROR("Parameter 'pbSpace' is NULL.");
        return;
    }

    if(elementSize == 0)
    {
        K_LOG_ERROR("Parameter 'elementSize' is zero.");
        return;
    }

    if(pbSpace->Element != NULL)
    {
        DeAllocFn(pbSpace->Element, pbSpace->Capacity * elementSize);
        pbSpace->Element = NULL;
    }

    pbSpace->Size = 0;
    pbSpace->Capacity = 0;
}

void FreePossibilitySpace(PossibilitySpace* pbSpace, size_t elementSize)
{
    DeAllocatePossibilitySpaceElements(pbSpace, elementSize);
    DeAllocFn(pbSpace, sizeof(PossibilitySpace));
}

void FreePossibilitySpaceArray(PossibilitySpace* pbSpaceArray, size_t pbSpaceCount, size_t elementSize)
{
    for(size_t i = 0; i < pbSpaceCount; ++i)
    {
        DeAllocatePossibilitySpaceElements(&pbSpaceArray[i], elementSize);
    }

    DeAllocFn(pbSpaceArray, pbSpaceCount * sizeof(PossibilitySpace));
}

PossibilitySpace* SortV2(void *array, size_t arraySize, size_t elementSize, size_t pbSpaceCount, SpaceIndexerFn indexerFn, void* indexerFnData, CompareFunction comparerFn, SortFunction sortFn)
{
    K_LOG_DEBUG("Starting SortV2...");

    PossibilitySpace* pbSpaces = CreatePossibilitySpaceArray(pbSpaceCount);

    if (pbSpaces == NULL)
    {
        K_LOG_ERROR("Failed to create PossibilitySpace array.");
        return NULL;
    }

    K_LOG_DEBUG("Counting possibility spaces...");
    if (!CountPossibilitySpaces(array, arraySize, elementSize, pbSpaceCount, indexerFn, indexerFnData, pbSpaces))
    {
        K_LOG_ERROR("Failed to count possibility spaces.");
        FreePossibilitySpaceArray(pbSpaces, pbSpaceCount, elementSize);
        return NULL;
    }

    K_LOG_DEBUG("Allocating elements for possibility spaces...");
    if (!AllocatePossibilitySpaceElements(elementSize, pbSpaceCount, pbSpaces))
    {
        K_LOG_ERROR("Failed to allocate elements for possibility spaces.");
        FreePossibilitySpaceArray(pbSpaces, pbSpaceCount, elementSize);
        return NULL;
    }

    K_LOG_DEBUG("Copying elements to possibility spaces...");
    if (!CopyElementsToPossibilitySpaces(array, arraySize, elementSize, pbSpaceCount, pbSpaces, indexerFn, indexerFnData))
    {
        K_LOG_ERROR("Failed to copy elements to possibility spaces.");
        FreePossibilitySpaceArray(pbSpaces, pbSpaceCount, elementSize);
        return NULL;
    }

    K_LOG_DEBUG("Sorting possibility spaces...");
    if (!SortPossibilitySpaces(pbSpaceCount, pbSpaces, elementSize, comparerFn, sortFn))
    {
        K_LOG_ERROR("Failed to sort possibility spaces.");
        FreePossibilitySpaceArray(pbSpaces, pbSpaceCount, elementSize);
        return NULL;
    }

    K_LOG_DEBUG("Sorting completed successfully.");
    return pbSpaces;
}

bool CountPossibilitySpaces(
    void *array,
    size_t arraySize,
    size_t elementSize,
    size_t pbSpaceCount,
    SpaceIndexerFn indexerFn,
    void* indexerFnData,
    PossibilitySpace* pbSpacesStoreArray
)
{
    if(!array)
    {
        K_LOG_ERROR("Parameter 'array' is NULL.");
        return false;
    }

    if(arraySize == 0)
    {
        K_LOG_ERROR("Parameter 'arraySize' is zero.");
        return false;
    }

    if(elementSize == 0)
    {
        K_LOG_ERROR("Parameter 'elementSize' is zero.");
        return false;
    }

    if(pbSpaceCount == 0)
    {
        K_LOG_ERROR("Parameter 'pbSpaceCount' is zero.");
        return false;
    }

    if(!indexerFn)
    {
        K_LOG_ERROR("Parameter 'indexerFn' is NULL.");
        return false;
    }

    if(!pbSpacesStoreArray)
    {
        K_LOG_ERROR("Parameter 'pbSpacesStoreArray' is NULL.");
        return false;
    }

    K_LOG_DEBUG("Setting initial values for PossibilitySpaces...");

    // Set initial values
    for (size_t i = 0; i < pbSpaceCount; ++i)
    {
        pbSpacesStoreArray[i].Element = NULL;
        pbSpacesStoreArray[i].Capacity = 0;
        pbSpacesStoreArray[i].Size = 0;
    }

    K_LOG_DEBUG("Getting elements indexes");

    for (size_t i = 0; i < arraySize; ++i)
    {
        K_LOG_DEBUG("Getting pointer of " SIZE_T_IDENTIFIER "th element", i);

        void* element = GetIndex(array, i, elementSize);

        K_LOG_DEBUG("Getting index for element " SIZE_T_IDENTIFIER, i);
        size_t index = indexerFn(element, indexerFnData);

        K_LOG_DEBUG("Element " SIZE_T_IDENTIFIER " has index " SIZE_T_IDENTIFIER, i, index);

        if (index >= pbSpaceCount)
        {
            K_LOG_ERROR("indexerFn returned invalid index: " SIZE_T_IDENTIFIER, index);
            return false;
        }

        pbSpacesStoreArray[index].Capacity++;
    }

    return true;
}

bool CopyElementsToPossibilitySpaces(
    void *array,
    size_t arraySize,
    size_t elementSize,
    size_t pbSpaceCount,
    PossibilitySpace* pbSpacesStoreArray,
    SpaceIndexerFn indexerFn,
    void* indexerFnData
)
{
    if(!array)
    {
        K_LOG_ERROR("Parameter 'array' is NULL.");
        return false;
    }

    if(arraySize == 0)
    {
        K_LOG_ERROR("Parameter 'arraySize' is zero.");
        return false;
    }

    if(elementSize == 0)
    {
        K_LOG_ERROR("Parameter 'elementSize' is zero.");
        return false;
    }

    if(!pbSpacesStoreArray)
    {
        K_LOG_ERROR("Parameter 'pbSpacesStoreArray' is NULL.");
        return false;
    }

    if(!indexerFn)
    {
        K_LOG_ERROR("Parameter 'indexerFn' is NULL.");
        return false;
    }

    for (size_t i = 0; i < arraySize; ++i)
    {
        void* element = GetIndex(array, i, elementSize);
        size_t index = indexerFn(element, indexerFnData);

        // Check if index is within bounds
        if (index >= pbSpaceCount)
        {
            K_LOG_ERROR("indexerFn returned out-of-bounds index: " SIZE_T_IDENTIFIER, index);
            return false;
        }

        PossibilitySpace* target = &pbSpacesStoreArray[index];

        // Out of bounds check for target space
        if (target->Size >= target->Capacity)
        {
            K_LOG_ERROR("Target space index " SIZE_T_IDENTIFIER " is out of bounds. Size: " SIZE_T_IDENTIFIER ", Capacity: " SIZE_T_IDENTIFIER, index, target->Size, target->Capacity);
            return false;
        }

        void* dest = GetIndex(target->Element, target->Size, elementSize);
        memcpy(dest, element, elementSize);
        target->Size++;
    }

    return true;
}

bool SortPossibilitySpaces(
    size_t pbSpaceCount,
    PossibilitySpace* pbSpacesStoreArray,
    size_t elementSize,
    CompareFunction comparerFn,
    SortFunction sortFn
)
{
    if(pbSpaceCount == 0)
    {
        K_LOG_ERROR("Parameter 'pbSpaceCount' is zero.");
        return false;
    }

    if(!pbSpacesStoreArray)
    {
        K_LOG_ERROR("Parameter 'pbSpacesStoreArray' is NULL.");
        return false;
    }

    if(elementSize == 0)
    {
        K_LOG_ERROR("Parameter 'elementSize' is zero.");
        return false;
    }

    if(!comparerFn)
    {
        K_LOG_ERROR("Parameter 'comparerFn' is NULL.");
        return false;
    }

    if(!sortFn)
    {
        K_LOG_ERROR("Parameter 'sortFn' is NULL.");
        return false;
    }

    for (size_t i = 0; i < pbSpaceCount; ++i)
    {
        if (pbSpacesStoreArray[i].Size > 1)
        {
            sortFn(
                pbSpacesStoreArray[i].Element,
                pbSpacesStoreArray[i].Size,
                elementSize,
                comparerFn
            );
        }
    }

    return true;
}

/// @brief 
/// @param pbSpaces 
/// @param pbSpaceCount 
/// @param arraySize 
/// @param elementSize 
/// @return Pointer allocated by AllocFn 
void *ToArray(PossibilitySpace *pbSpaces, size_t pbSpaceCount, size_t arraySize, size_t elementSize)
{
    void *arr = AllocFn(arraySize * elementSize);

    if(arr == NULL)
    {
        return arr;
    }

    size_t index = 0;
    for (size_t i = 0; i < pbSpaceCount; i++)
    {
        PossibilitySpace space = pbSpaces[i];

        void *arrPos = GetIndex(arr, index, elementSize);
        memcpy(arrPos, space.Element, space.Size * elementSize);

        index += space.Size;
    }

    return arr;
}
