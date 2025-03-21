#include "TestGlobal.h"
#include "KOKESort.h"

int main(void)
{
    int arr[] = {123456, 123459, 123458, 123453};
    void* sortedArr = SortV1(arr, sizeof(arr)/sizeof(int), sizeof(int), intComparer);

    if(!sortedArr)
    {
        perror("SortV1 result is not valid.");
        return EXIT_FAILURE;
    }

    FreeMemory(sortedArr);

    #ifdef WINDOWS
    // Note: On Windows, even after calling free(), the heap memory might not appear 
    // to be released immediately. This is because the C runtime often keeps the freed 
    // memory in its internal pool for performance reasons, and it may not return it 
    // to the OS right away. This behavior is normal and doesn't necessarily indicate 
    // a memory leak. Tools like _CrtDumpMemoryLeaks or Address Sanitizer can help 
    // detect actual memory leaks or heap corruption.
    return EXIT_SUCCESS;
    #else
    int* number = GetIndex(sortedArr, 0, sizeof(int));

    if(!sortedArr || !number || *number != 123453)
    {
        return EXIT_SUCCESS;
    }

    printf("%p -> %d", number, *number);

    return EXIT_FAILURE;
    #endif
}