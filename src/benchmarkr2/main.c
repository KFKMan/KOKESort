#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <csv.h>
#include "KOKESort.h"
#include "KOKESortV2.h"
#include "master.h"
#include "file.h"
#include "advtime.h"

#define MAX_NUM_ELEMENTS 10000000
#define MAX_NUM_VALUE 9223372036854775807

#define DATA_TYPE long long
#define DATA_TYPE_SPECIFIER "%lld"
#define DATA_TYPE_PARSER atoll

#define MAX_SORT_ALG_COUNT 100
#define MAX_LINE_LENGTH 8192

#define MAX_BENCHNAME_LENGTH 8192

void generate_filename(char *buffer, size_t buffer_size, const char *part1, const char *part2, DATA_TYPE number)
{
    snprintf(buffer, buffer_size, "%s%s" DATA_TYPE_SPECIFIER ".csv", part1, part2, number);
}

void append_to_csv(const char *filename, DATA_TYPE id, long long timestamp)
{
    FILE *file = fopen(filename, "a");
    if (!file)
    {
        file = fopen(filename, "w");
        if (!file)
        {
            perror("File Creation Error");
            return;
        }
    }

    fprintf(file, DATA_TYPE_SPECIFIER ",%lld\n", id, timestamp);
    fclose(file);
}

int valComparer(const void *val1ptr, const void *val2ptr)
{
    const DATA_TYPE val1 = *(const DATA_TYPE *)val1ptr;
    const DATA_TYPE val2 = *(const DATA_TYPE *)val2ptr;
    if (val1 > val2)
    {
        return 1;
    }
    if (val1 == val2)
    {
        return 0;
    }
    return -1;
}

const int SpaceCount = 100;

DATA_TYPE Divider;

size_t indexer(const void *valptr)
{
    const DATA_TYPE val1 = *(const DATA_TYPE *)valptr;
    return val1 / Divider;
}

void formatDuration(long long elapsed)
{
    double duration_ms = elapsed / 1000000.0;   // Nanosecond -> Milisecond
    double duration_sec = duration_ms / 1000.0; // Milisecond -> Saniye
    double duration_min = duration_sec / 60.0;  // Second -> Minute
    double duration_hour = duration_min / 60.0; // Minute -> Hour

    if (duration_ms < 1)
    {
        printf("Duration: %lld ns\n", elapsed);
    }
    else if (duration_sec < 1)
    {
        printf("Duration: %.3f ms\n", duration_ms);
    }
    else if (duration_min < 1)
    {
        printf("Duration: %.3f s\n", duration_sec);
    }
    else if (duration_hour < 1)
    {
        printf("Duration: %.3f min\n", duration_min);
    }
    else
    {
        printf("Duration: %.3f h\n", duration_hour);
    }

    // Anyway print ms for advanced info
    printf("Duration: %.3f ms\n", duration_ms);
}

void benchmarkSortFunction(DATA_TYPE id, const DATA_TYPE *data, size_t size, SortFunctionEntry sortFnEntry)
{
    DATA_TYPE *copy = (DATA_TYPE *)malloc(size * sizeof(DATA_TYPE));
    memcpy(copy, data, size * sizeof(DATA_TYPE));

    printf("%s - Started \n", sortFnEntry.name);
    long long start, end, elapsed;
    start = get_time_ns();
    sortFnEntry.function(copy, size, sizeof(DATA_TYPE), valComparer);
    end = get_time_ns();

    elapsed = end - start;

    printf("%s | " SIZE_T_IDENTIFIER " --> ", sortFnEntry.name, size);
    formatDuration(elapsed);

    char saveFile[R2_MAX_FILENAME];
    generate_filename(saveFile, sizeof(saveFile), "result", sortFnEntry.name, id);
    append_to_csv(saveFile, id, elapsed);

    free(copy);
}

int GetBool(const char *question)
{
    int x = SUCCESS;
    printf(question);
    scanf("%d", &x);
    return x;
}

void SortV1SelfNonOpt(void *arr, size_t size, size_t elementSize, CompareFunction comparer)
{
    SortV1Self(arr, size, elementSize, comparer);
}

PossibilitySpace** fSpace = NULL;

void SortV2NonOpt(void *arr, size_t size, size_t elementSize, CompareFunction comparer)
{
    fSpace = SortV2(arr, size, elementSize, SpaceCount, indexer, comparer, qsort);
}

void AddItemToList(SortFunctionEntry *array, size_t *count, SortFunctionEntry entry)
{
    array[*count] = entry;
    *count = *count + 1;
}

int isFunctionInList(const char *functionName, char functionNames[MAX_SORT_ALG_COUNT][MAX_LINE_LENGTH], size_t functionCount)
{
    for (size_t i = 0; i < functionCount; i++)
    {
        if (strcmp(functionNames[i], functionName) == 0)
        {
            return 1;
        }
    }
    return 0;
}

size_t readFunctionNamesFromFile(const char *filename, char functionNames[MAX_SORT_ALG_COUNT][MAX_LINE_LENGTH], size_t maxFunctions, int *errorState)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("File Can't Opened");
        *errorState = FAIL;
        return 0;
    }

    size_t count = 0;
    while (count < maxFunctions && fgets(functionNames[count], MAX_LINE_LENGTH, file))
    {
        functionNames[count][strcspn(functionNames[count], "\n")] = '\0';
        count++;
    }

    fclose(file);
    return count;
}

int main(int argc, char **argv)
{
    printf("Benchmark R2 App Started V1.0.0\n");

    Divider = MAX_NUM_VALUE / SpaceCount; // This is needed for Configuring Indexer Function, SortV2 using it.

    printf("Functions Initializing... \n");

    size_t functionCount = 0;
    SortFunctionEntry *sortingFunctions = malloc(sizeof(SortFunctionEntry) * MAX_SORT_ALG_COUNT);

    SortFunctionEntry __sortingFunctions[] = _SortingFunctions;

    for (size_t i = 0; i < sizeof(__sortingFunctions) / sizeof(__sortingFunctions[0]); i++)
    {
        AddItemToList(sortingFunctions, &functionCount, __sortingFunctions[i]);
    }

    SortFunctionEntry sortV1 = {"SortV1S", SortV1SelfNonOpt};
    SortFunctionEntry sortV2 = {"SortV2S", SortV2NonOpt};

    AddItemToList(sortingFunctions, &functionCount, sortV1);
    AddItemToList(sortingFunctions, &functionCount, sortV2);

    char functionNamesFromFile[MAX_SORT_ALG_COUNT][MAX_LINE_LENGTH];
    int errState = SUCCESS;
    size_t fileFunctionCount = readFunctionNamesFromFile("functions.txt", functionNamesFromFile, MAX_SORT_ALG_COUNT, &errState);

    if (errState == SUCCESS)
    {
        size_t newCount = 0;
        for (size_t i = 0; i < functionCount; i++)
        {
            if (isFunctionInList(sortingFunctions[i].name, functionNamesFromFile, fileFunctionCount))
            {
                sortingFunctions[newCount++] = sortingFunctions[i];
            }
        }
        functionCount = newCount;
    }

    printf("Functions Initialized \n");

    size_t fileCount = 0;
    FileEntry *entries = GetFileEntrys(&fileCount);

    printf(SIZE_T_IDENTIFIER " file founded", fileCount);

    for (size_t currentFileIndex = 0; currentFileIndex < fileCount; currentFileIndex++)
    {
        FileEntry currentFile = entries[currentFileIndex];

        const char *filepath = currentFile.filename;

        printf("Processing File %s \n", currentFile.filename);

        char *row;
        CsvHandle handle = CsvOpen2(filepath, ' ', '"', '\\');
        if (!handle)
        {
            printf("Failed to open file: %s\n", filepath);
            return 11;
        }

        printf("File Opened \n");

        printf("Reading and Benching... \n");

        while ((row = CsvReadNextRow(handle)))
        {
            DATA_TYPE *array = malloc(MAX_NUM_ELEMENTS * sizeof(DATA_TYPE));
            if (!array)
            {
                perror("Memory allocation failed");
                CsvClose(handle);
                return 12;
            }

            size_t index = 0;
            const char *col;
            while ((col = CsvReadNextCol(row, handle)))
            {
                if (index >= MAX_NUM_ELEMENTS)
                {
                    perror("Array size exceeds maximum limit\n");
                    free(array);
                    CsvClose(handle);
                    return 13;
                }
                array[index++] = DATA_TYPE_PARSER(col);
            }

            if (index == 0)
            {
                printf("Array is empty skipped");
                free(array);
                continue;
            }

            for (size_t i = 0; i < functionCount; i++)
            {
                benchmarkSortFunction(currentFile.id, array, index, sortingFunctions[i]);
            }

            FreePossibilitySpaceArray(fSpace, SpaceCount, sizeof(DATA_TYPE));

            free(array);
        }
    }

    return 0;
}