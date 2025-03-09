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
            printf("Format Error On %d -> Target \n", rows);
            return HandleError();
        }

        int target = atoi(col);

        if(!(col = CsvReadNextCol(row, handle)))
        {
            printf("Format Error On %d -> Expected \n", rows);
            return HandleError();
        }

        int expected = atoi(col);

        size_t result = FindInsertIndexBS((void*)arr, count, (void*)&target, intComparer, sizeof(int));
        test_count++;

        if (result == expected)
        {
            printf("Test %d PASSED: target = %d, expected = %d\n", test_count, target, expected);
        }
        else
        {
            printf("Test %d FAILED: target = %d, expected = %d, getted = " SIZE_T_IDENTIFIER "\n", test_count, target, expected, result);
            failed_count++;
        }

        rows++;
    }

    CsvClose(handle);

    printf("\nTotal test: %d, Passed: %d, Failed: %d\n", test_count, test_count - failed_count, failed_count);

    return (failed_count == 0 && test_count != 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
