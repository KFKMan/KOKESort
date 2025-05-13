#pragma once

typedef int GenerateValueType;

const int MaxValue = 10000000;

//For KOKESortV2
int SpaceCount = 100;

GenerateValueType Divider = MaxValue / SpaceCount;

void SetSpaceCount(int spaceCount)
{
    SpaceCount = spaceCount;
    Divider = MaxValue / SpaceCount;
}