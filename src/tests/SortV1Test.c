#include "TestGlobal.h"
#include "KOKESort.h"

#define MAX_LINE_LEN 2048
#define MAX_NUM_ELEMENTS 1000

int main(void)
{
    FILE* fp;
    int fileState = TryOpenTestDataFile(&fp, "r");

    if(fileState != 0)
    {
        perror("File can't opened: ");
        perror(GetFilename(__FILE__));
        return EXIT_FAILURE;
    }

    char line[MAX_LINE_LEN];
    int test_count = 0, failed_count = 0;

    // Reading all test data, row by row
    while (fgets(line, sizeof(line), fp))
    {
        // Deleting new line characters
        line[strcspn(line, "\r\n")] = 0;

        // Skipping empty lines
        if (line[0] == '\0')
        {
            continue;
        }

        // Getting data
        // 1. Array
        // 2. Expected value
        char* linePtr = NULL;
        char *array_str = strtok_s(line, ";", &linePtr);
        char *expected_str = strtok_s(NULL, ";", &linePtr);

        // If any of them is null, continue
        if (!array_str || !expected_str)
        {
            fprintf(stderr, "Format error: %s\n", line);
            return EXIT_FAILURE;
            //continue;
        }

        #ifdef DEBUG
        printf("array_str: %s\n", array_str);
        printf("expected_str: %s\n", expected_str);
        #endif

        // Reading array with " " join/split character
        int arr[MAX_NUM_ELEMENTS];
        char* arrContext = NULL;
        size_t count = ReadArray(arr, array_str, " ", &arrContext, MAX_NUM_ELEMENTS);

        int expectedArr[MAX_NUM_ELEMENTS];
        char* expectedContext = NULL;
        size_t expectedCount = ReadArray(expectedArr, expected_str, " ", &expectedContext, MAX_NUM_ELEMENTS);

        // Calling function and getting result
        void* result = SortV1(arr, count, intComparer, sizeof(int));
        test_count++;

        if (ArrayEqual(result, expectedArr, count, intComparer, sizeof(int)) == 0)
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
    }

    fclose(fp);

    printf("\nTotal test: %d, Passed: %d, Failed: %d\n", test_count, test_count - failed_count, failed_count);

    return (failed_count == 0 && test_count != 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}