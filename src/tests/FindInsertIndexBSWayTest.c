#include "TestGlobal.h"
#include "KOKESort.h"

int TestNullArray()
{
    int element = 5;
    size_t result;
    int state = FindInsertIndexBS(NULL, 0, &element, intComparer, sizeof(int), &result);
    if(state == 1 && result == 0)
    {
        return 1;
    }
    return 0;
}

int TestZeroSizeArray()
{
    int arr[] = {10, 15, 20, 25};
    int element = 18;
    size_t result;
    int state = FindInsertIndexBS(arr, 0, &element, intComparer, sizeof(int), &result);
    if(state == 1 && result == 0)
    {
        return 1;
    }
    return 0;
}

int main(void)
{
    if(TestNullArray() == 1 && TestZeroSizeArray() == 1)
    {
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}