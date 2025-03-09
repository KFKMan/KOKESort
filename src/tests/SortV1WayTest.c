#include "TestGlobal.h"
#include "KOKESort.h"

int TestNullArray()
{
    void* result = SortV1(NULL, 10, intComparer, sizeof(int));

    if(result == NULL)
    {
        return 1;
    }

    free(result);
    return 0;
}

int TestZeroSizeArray()
{
    int arr[] = {10, 20, 40, 25};
    void* result = SortV1(arr, 0, intComparer, sizeof(int));

    if(result == NULL)
    {
        return 1;
    }

    free(result);
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