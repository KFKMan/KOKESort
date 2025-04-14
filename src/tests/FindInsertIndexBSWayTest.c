#include "TestGlobal.h"
#include "KOKESort.h"

int TestNullArray()
{
    int element = 5;
    size_t result = FindInsertIndexBS(NULL, 0, &element, intComparer, sizeof(int));
    if(result == 0)
    {
        return SUCCESS;
    }
    return FAIL;
}

int TestZeroSizeArray()
{
    int arr[] = {10, 15, 20, 25};
    int element = 18;
    size_t result = FindInsertIndexBS(arr, 0, &element, intComparer, sizeof(int));
    if(result == 0)
    {
        return SUCCESS;
    }
    return FAIL;
}

int main(void)
{
    if(TestNullArray() == SUCCESS && TestZeroSizeArray() == SUCCESS)
    {
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}
