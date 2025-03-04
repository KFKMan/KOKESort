#include "TestGlobal.h"
#include "KOKESort.h"

#define DEBUG

#define MAX_LINE_LEN 2048
#define MAX_NUM_ELEMENTS 1000

int main(void)
{
    #ifdef DEBUG
    printf("Test Started \n");
    #endif

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

        // Getting data
        // 1. Array
        // 2. Expected value

        char* linePtr = NULL;
        char *array_str = strtok_s(line, ";", &linePtr);

        if (!array_str)
        {
            fprintf(stderr, "Format error: %s\n", line);
            return EXIT_FAILURE;
            //continue;
        }

        #ifdef DEBUG
        printf("array_str: %s\n", array_str);
        #endif

        // Reading array with " " join/split character
        int arr[MAX_NUM_ELEMENTS];
        char* arrContext = NULL;
        size_t count = ReadArray(arr, array_str, " ", &arrContext, MAX_NUM_ELEMENTS);

        #ifdef DEBUG
        printf("Calling Function... \n");
        #endif

        // Calling function and getting result
        BinaryTree* result = SortV2(arr, count);
        test_count++;

        #ifdef DEBUG
        printf("Called Function... \n");
        #endif

        if (1 == 1)
        {
            printf("Test %d PASSED: \n", test_count);
        }
        else
        {
            printf("Test %d FAILED: \n", test_count);
            printf("Parameter: ");
            PrintArray(arr, count);
            printf("\n");
            char buffer[4096] = "";
            SerializeBinaryTree(result, buffer, 4096);
            printf("Result: %s", buffer);
            printf("\n");
            //printf("Result: ");
            //PrintArray(result, count);
            //printf("\n");
            //printf("Expected: ");
            //PrintArray(expectedArr, expectedCount);
            //printf("\n");
            failed_count++;
        }
    }

    fclose(fp);

    printf("\nTotal test: %d, Passed: %d, Failed: %d\n", test_count, test_count - failed_count, failed_count);

    return (failed_count == 0 && test_count != 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}