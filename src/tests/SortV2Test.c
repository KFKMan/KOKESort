#include "TestGlobal.h"
#include "KOKESortV2.h"
#include "csv.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE_LEN 2048
#define MAX_NUM_ELEMENTS 1000

typedef struct indexerData{
    int Divider;
} IndexerData;

size_t indexerFunction(const void* val, const void* data)
{
    const int numVal = *(const int*)val;
    const IndexerData inxData = *(const IndexerData*)data;
    
    return numVal / inxData.Divider;
}

int GetMax(int* arr, size_t count)
{
    int max = arr[0];
    for (size_t i = 1; i < count; ++i)
    {
        if (arr[i] > max)
        {
            max = arr[i];
        }
    }
    return max;
}

int GetMin(int* arr, size_t count)
{
    int min = arr[0];
    for (size_t i = 1; i < count; ++i)
    {
        if (arr[i] < min)
        {
            min = arr[i];
        }
    }
    return min;
}

int main(void)
{
    int test_count = 0, failed_count = 0;

    char* row;
    int rows = 1;
    CsvHandle handle = CsvOpen2(GetTestDataFile(), ';', '"', '\\');
    
    while ((row = CsvReadNextRow(handle)))
    {   
        const char* col;

        if(!(col = CsvReadNextCol(row, handle)))
        {
            printf("Format Error On %d -> Array \n", rows);
            return HandleError();
        }

        char* tempCol = strdup(col);

        int arr[MAX_NUM_ELEMENTS];
        char* arrContext = NULL;
        size_t count = ReadArray(arr, tempCol, " ", &arrContext, MAX_NUM_ELEMENTS);

        free(tempCol);

        if(!(col = CsvReadNextCol(row, handle)))
        {
            printf("Format Error On %d -> Expected \n", rows);
            return HandleError();
        }

        tempCol = strdup(col);

        int expectedArr[MAX_NUM_ELEMENTS];
        char* expectedContext = NULL;
        size_t expectedCount = ReadArray(expectedArr, tempCol, " ", &expectedContext, MAX_NUM_ELEMENTS);

        free(tempCol);

        int max = GetMax(arr, count);

        //WARNING: This test is not valid for negative numbers, as it assumes all numbers are positive.

        int spaceCount = 10;

        IndexerData inxData = 
        { 
            .Divider = (max / spaceCount) + 1
        };

        K_LOG_DEBUG("Test %d: Divider = %d, Max = %d, SpaceCount = %d", rows, inxData.Divider, max, spaceCount);

        PossibilitySpace* result = SortV2(arr, count, sizeof(int), spaceCount, indexerFunction, &inxData, intComparer, qsort);
        void* resultArr = ToArray(result, spaceCount, count, sizeof(int));

        test_count++;

        if (ArrayEqual(resultArr, expectedArr, count, intComparer, sizeof(int)) == 0)
        {
            printf("Test %d PASSED: \n", test_count);
        }
        else
        {
            printf("Test %d FAILED: \n", test_count);
            printf("Parameter: ");
            PrintArray(arr, count);
            printf("\n");
            printf("Result: ");
            PrintArray(result, count);
            printf("\n");
            printf("Expected: ");
            PrintArray(expectedArr, expectedCount);
            printf("\n");
            failed_count++;
        }

        FreePossibilitySpaceArray(result, spaceCount, sizeof(int));

        rows++;
    }

    CsvClose(handle);

    printf("\nTotal test: %d, Passed: %d, Failed: %d\n", test_count, test_count - failed_count, failed_count);

    return (failed_count == 0 && test_count != 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
