#pragma once

typedef int GenerateValueType;

const int MaxValue = 10000000;

//For KOKESortV2
static int SpaceCount = 100;

static GenerateValueType Divider = MaxValue / SpaceCount;

static PossibilitySpace* LastSortData = nullptr;

void ClearLastSortData(size_t elementCount, size_t elementSize)
{
    if (LastSortData != nullptr)
    {
        FreePossibilitySpaceArray(LastSortData, SpaceCount, elementSize);
        LastSortData = nullptr;
    }
    // Clear other resources if needed
}

void SetSpaceCount(int spaceCount)
{
    SpaceCount = spaceCount;
    Divider = (MaxValue / SpaceCount) + 1; // +1 for fixing floating and zero values
}