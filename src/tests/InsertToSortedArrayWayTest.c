#include "TestGlobal.h"
#include "KOKESort.h"

int TestNullArray()
{
    int element = 18;
    void* result = InsertToSortedArray(NULL, 0, &element, intComparer, sizeof(int));
    if(result == NULL)
    {
        return 0;
    }

    const int* resultIntPtr = (const int*)result;

    if(*resultIntPtr == element)
    {
        return 1;
    }

    free(result);
    return 0;
}

int TestZeroSizeArray()
{
    int arr[] = {10, 15, 20, 25};
    int element = 18;
    void* result = InsertToSortedArray(arr, 0, &element, intComparer, sizeof(int));
    if(result == NULL)
    {
        return 0;
    }

    const int* resultIntPtr = (const int*)result;

    if(*resultIntPtr == element)
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
