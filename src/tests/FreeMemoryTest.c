#include "TestGlobal.h"
#include "KOKESort.h"

int main(void)
{
    int arr[] = {123456, 123459, 123458, 123457};
    void* sortedArr = SortV1(arr, sizeof(arr)/sizeof(int), intComparer, sizeof(int));

    if(!sortedArr)
    {
        perror("SortV1 result is not valid.");
        return EXIT_FAILURE;
    }

    FreeMemory(sortedArr);

    int* number = GetIndex(sortedArr, 0, sizeof(int));

    if(*number != 123456)
    {
        return EXIT_SUCCESS;
    }

    printf("%p -> %d", number, *number);

    return EXIT_FAILURE;
}