#include "TestGlobal.h"
#include "KOKESort.h"

int main(void)
{
    int arr[] = {10, 15, 13, 24};
    void* sortedArr = SortV1(arr, sizeof(arr)/sizeof(int), intComparer, sizeof(int));

    if(!sortedArr)
    {
        perror("SortV1 result is not valid.");
        return EXIT_FAILURE;
    }

    FreeMemory(sortedArr);

    int* number = GetIndex(sortedArr, 0, sizeof(int));

    if(*number != 10)
    {
        return EXIT_SUCCESS;
    }

    printf("%p -> %d", number, *number);

    return EXIT_FAILURE;
}