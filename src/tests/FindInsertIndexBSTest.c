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
        perror(__FILE__);
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

        char* linePtr = line;

        // Getting data
        // 1. Array
        // 2. Target value
        // 3. Expected value
        char *array_str = strtok_s(line, ";", &linePtr);
        char *target_str = strtok_s(NULL, ";", &linePtr);
        char *expected_str = strtok_s(NULL, ";", &linePtr);

        // If any of them is null, continue
        if (!array_str || !target_str || !expected_str)
        {
            fprintf(stderr, "Format error: %s\n", line);
            return EXIT_FAILURE;
            //continue;
        }

        // Converting to integers
        int target = atoi(target_str);
        int expected = atoi(expected_str);

        // Reading array with " " join/split character
        int arr[MAX_NUM_ELEMENTS];
        char* arrContext = NULL;
        size_t count = ReadArray(arr, array_str, " ", &arrContext, MAX_NUM_ELEMENTS);

        // Calling function and getting result
        size_t result = FindInsertIndexBS(arr, count, target);
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
    }

    fclose(fp);

    printf("\nTotal test: %d, Passed: %d, Failed: %d\n", test_count, test_count - failed_count, failed_count);

    return (failed_count == 0 && test_count != 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
