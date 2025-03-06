#include "ubench.h"
#include "KOKESort.h"

//#define DEBUG

#define MAX_LINE_LEN 100000
#define MAX_NUM_ELEMENTS 5000
#define MAX_TESTS 500

UBENCH_STATE();

int arrays[MAX_TESTS][MAX_NUM_ELEMENTS];
size_t array_sizes[MAX_TESTS];
int count = 0;

int intComparer(const void* val1ptr, const void* val2ptr)
{
    const int val1 = *(const int*)val1ptr;
    const int val2 = *(const int*)val2ptr;
    if(val1 > val2)
    {
        return 1;
    }
    if(val1 == val2)
    {
        return 0;
    }
    return -1;
}

void printArray(int* arr, int size)
{
    printf("Array Printing");
    for(int i = 0; size > i; i++)
    {
        printf("Element %d: %d", i, arr[i]);
    }
}

UBENCH(Sort, int)
{
    for(int i = 0; count > i; i++)
    {
        int* sorted = SortV1(arrays[i], array_sizes[i], intComparer);
        #ifdef DEBUG
        printArray(sorted, array_sizes[i]);
        #endif
        free(sorted);
    }    
}

int TryOpenTestDataFile(FILE** fp, const char* mode) {
    *fp = fopen("testdata.txt", mode);
    return *fp ? 0 : -1;
}

size_t ReadArray(int* arr, const char* array_str, const char* delim, char** context, size_t max_elements) {
    size_t count = 0;
    char* token = strtok_s((char*)array_str, delim, context);
    while (token && count < max_elements) {
        arr[count++] = atoi(token);
        token = strtok_s(NULL, delim, context);
    }
    return count;
}

int main(int argc, const char *const argv[]) 
{
    debugPrintFast("App started");
    FILE* fp;
    int fileState = TryOpenTestDataFile(&fp, "r");

    if (fileState != 0) 
    {
        debugPrintFast("File can't be opened");
        perror("File can't be opened: ");
        perror(__FILE__);
        return EXIT_FAILURE;
    }

    char line[MAX_LINE_LEN];
    int count = 0;

    debugPrintFast("Starting to read");

    while (fgets(line, sizeof(line), fp)) {
        debugPrintFast("Reading line...");
        line[strcspn(line, "\r\n")] = 0;

        if (line[0] == '\0') continue;

        char* linePtr = NULL;
        char* array_str = strtok_s(line, ";", &linePtr);

        if (!array_str) {
            debugPrintFast("Format error");
            fprintf(stderr, "Format error: %s\n", line);
            fclose(fp);
            return EXIT_FAILURE;
        }

        if (count >= MAX_TESTS) {
            debugPrintFast("Too many tests, increase MAX_TESTS\n");
            fprintf(stderr, "Too many tests, increase MAX_TESTS\n");
            fclose(fp);
            return EXIT_FAILURE;
        }

        char* arrContext = NULL;
        array_sizes[count] = ReadArray(arrays[count], array_str, " ", &arrContext, MAX_NUM_ELEMENTS);
        count++;
    }

    fclose(fp);

    printf("Test Data Count %d \n", count);

    #ifdef DEBUG
    for (int i = 0; i < count; i++) 
    {
        printf("Test %d: ", i + 1);
        for (size_t j = 0; j < array_sizes[i]; j++) 
        {
            printf("%d ", arrays[i][j]);
        }
        printf("\n");
    }
    #endif

    return ubench_main(argc, argv);
}