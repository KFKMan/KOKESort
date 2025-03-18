#include <KOKESortV2.h>

// Steps
// 1- Allocate and Divide Space
// 2- Create Backup Space
// 3- Start To Use

#define LinkedListArch


typedef size_t (*SpaceIndexerFn)(const void *);

const size_t PointerSize = sizeof(int *);

typedef struct WayList
{
    void *Element;

    #ifdef LinkedListArch
    struct WayList *NextElement;
    // Maybe functionallity for Previous Element?
    #else
    int Size;
    #endif
} WayList;

typedef struct PossibilitySpace
{
    void *Element;
    WayList *NextElement;

} PossibilitySpace;

size_t GetPossibilitySpaceSize(size_t elementSize)
{
    return elementSize + PointerSize;
}

size_t GetWaySpaceSize(size_t elementSize)
{
    return elementSize + PointerSize;
}

void *SortV2(void *array, size_t arraySize, size_t elementSize, size_t pbSpaceCount, SpaceIndexerFn indexerFn, size_t waySpaceCount, CompareFunction comparerFn)
{
    // We need to store allocation info for deallocation.
    // TODO: Simple Comparer Support Need To Be Added
    // TODO: Same Element Support Need To Be Added
    size_t pbSpacePerSize = GetPossibilitySpaceSize(elementSize);
    size_t waySpacePerSize = GetWaySpaceSize(elementSize);

    void *pbSpace = calloc(pbSpaceCount, pbSpacePerSize);
    void *waySpace = calloc(waySpaceCount, waySpacePerSize);

    size_t currentWaySpace = 0;

    for (size_t i = 0; i < arraySize; i++)
    {
        void *currentElement = GetIndex(array, i, elementSize);
        size_t indexerIndex = indexerFn(currentElement);

        PossibilitySpace *pb = GetIndex(pbSpace, indexerIndex, pbSpacePerSize);

        if (pb->Element == NULL)
        {
            //pb->Element = currentElement; //?? exceptionable??
            memcpy(pb->Element, currentElement, elementSize);

            continue;
        }

        if (pb->NextElement == NULL)
        {
            WayList *way = GetIndex(waySpace, currentWaySpace, waySpacePerSize);
            //way->Element = currentElement; //?? exceptionable??
            memcpy(way->Element, currentElement, elementSize);

            pb->NextElement = way;

            currentWaySpace++;
            continue;
        }

        WayList *way = pb->NextElement;

        if (way->NextElement == NULL)
        {
            if (comparerFn(way->Element, currentElement) > 0)
            {
                WayList *crrWay = GetIndex(waySpace, currentWaySpace, waySpacePerSize);
                //crrWay->Element = currentElement; //?? exceptionable??
                memcpy(crrWay->Element, currentElement, elementSize);

                crrWay->NextElement = way;
                pb->NextElement = crrWay;

                currentWaySpace++;
                continue;
            }
            else
            {
                WayList *crrWay = GetIndex(waySpace, currentWaySpace, waySpacePerSize);
                //crrWay->Element = currentElement; //?? exceptionable??
                memcpy(crrWay->Element, currentElement, elementSize);

                way->NextElement = crrWay;

                currentWaySpace++;
                continue;
            }
        }

        way = way->NextElement;

        while (way->NextElement != NULL)
        {
            if (comparerFn(way->NextElement->Element, currentElement) > 0)
            {
                WayList *crrWay = GetIndex(waySpace, currentWaySpace, waySpacePerSize);
                //crrWay->Element = currentElement; //?? exceptionable??
                memcpy(crrWay->Element, currentElement, elementSize);

                crrWay->NextElement = way->NextElement;
                way->NextElement = crrWay;

                currentWaySpace++;
                continue;
            }

            way = way->NextElement;
        }

        WayList *cWay = GetIndex(waySpace, currentWaySpace, waySpacePerSize);
        //cWay->Element = currentElement; //?? exceptionable??
        memcpy(cWay->Element, currentElement, elementSize);

        way->NextElement = cWay;

        currentWaySpace++;
    }

    return pbSpace;
}
