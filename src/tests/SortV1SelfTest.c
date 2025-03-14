#include "TestGlobal.h"
#include "KOKESort.h"
#include "csv.h"

#define MAX_LINE_LEN 2048
#define MAX_NUM_ELEMENTS 1000

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

        int result = SortV1Self(arr, count, intComparer, sizeof(int));
        test_count++;

        if (result == SUCCESS && ArrayEqual(arr, expectedArr, count, intComparer, sizeof(int)) == 0)
        {
            printf("Test %d PASSED: \n", test_count);
        }
        else
        {
            printf("Test %d FAILED: \n", test_count);
            printf("Parameter: ");
            PrintArray(arr, count);
            printf("\n");
            printf("Expected: ");
            PrintArray(expectedArr, expectedCount);
            printf("\n");
            failed_count++;
        }

        rows++;
    }

    CsvClose(handle);

    printf("\nTotal test: %d, Passed: %d, Failed: %d\n", test_count, test_count - failed_count, failed_count);

    return (failed_count == 0 && test_count != 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}