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
            printf("Format Error On %d -> Element \n", rows);
            return HandleError();
        }

        int element = atoi(col);

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

        void* result = InsertToSortedArray(arr, count, (void*)&element, intComparer, sizeof(int));
        test_count++;

        if (ArrayEqual(result, expectedArr, expectedCount, intComparer, sizeof(int)) == 0)
        {
            printf("Test %d PASSED: \n", test_count);
        }
        else
        {
            printf("Test %d FAILED: \n", test_count);
            printf("Array: ");
            PrintArray(arr, count);
            printf("\n");
            printf("Element: %d", element);
            printf("\n");
            printf("Result: ");
            PrintArray(result, count);
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
