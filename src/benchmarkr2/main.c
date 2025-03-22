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

#define MAX_BENCHNAME_LENGTH 8192

const char* int_to_string(char buffer[], size_t buffer_size, int number) {
    snprintf(buffer, buffer_size, "%d", number);
    return buffer;
}

void generate_filename(char* buffer, size_t buffer_size, const char* part1, const char* part2, int number) {
    snprintf(buffer, buffer_size, "%s%s%d.csv", part1, part2, number);
}

void append_to_csv(const char* filename, int id, long long timestamp) 
{
    FILE* file = fopen(filename, "a");
    if (!file) 
    {
        file = fopen(filename, "w");
        if (!file) {
            perror("File Creation Error");
            return;
        }
    }

    fprintf(file, "%d,%lld\n", id, timestamp);
    fclose(file);
}

int intComparer(const void *val1ptr, const void *val2ptr)
{
    const int val1 = *(const int *)val1ptr;
    const int val2 = *(const int *)val2ptr;
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

const int MaxElementCount = 100000000;
const int SpaceCount = 1000;

int Divider;

size_t indexer(const void *valptr)
{
    const int val1 = *(const int *)valptr;
    return val1 / Divider;
}

void formatDuration(long long elapsed)
{
    double duration_ms = elapsed / 1000000.0;     // Nanosecond -> Milisecond
    double duration_sec = duration_ms / 1000.0;   // Milisecond -> Saniye
    double duration_min = duration_sec / 60.0;    // Second -> Minute
    double duration_hour = duration_min / 60.0;   // Minute -> Hour

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

    //Anyway print ms for advanced info
    printf("Duration: %.3f ms\n", duration_ms);
}

void benchmarkSortFunction(int id, const int *data, size_t size, SortFunctionEntry sortFnEntry)
{
    int *copy = (int *)malloc(size * sizeof(int));
    memcpy(copy, data, size * sizeof(int));

    printf("%s - Started \n", sortFnEntry.name);
    long long start, end, elapsed;
    start = get_time_ns();
    sortFnEntry.function(copy, size, sizeof(int), intComparer);
    end = get_time_ns();

    elapsed = end - start;

    printf("%s | %zu --> ", sortFnEntry.name, size);
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

void SortV2NonOpt(void *arr, size_t size, size_t elementSize, CompareFunction comparer)
{
    PossibilitySpace **pbSpaces = SortV2(arr, size, sizeof(int), SpaceCount, indexer, intComparer);
}

// Ana fonksiyon
int main(int argc, char **argv)
{
    printf("Benchmark R2 App Started V1.0.0\n");

    Divider = MaxElementCount / SpaceCount; // This is needed for Configuring Indexer Function, SortV2 using it.

    printf("Write %d for TRUE, %d for FALSE \n", SUCCESS, FAIL);
    int useV1 = GetBool("Use SortV1: ");

    printf("Functions Initializing... \n");

    size_t count = 0;

    SortFunctionEntry sortingFunctions[] = _SortingFunctions;
    size_t numAlgorithms = sizeof(sortingFunctions) / sizeof(sortingFunctions[0]);

    SortFunctionEntry sortV1 = {"SortV1", SortV1SelfNonOpt};
    SortFunctionEntry sortV2 = {"SortV2", SortV2NonOpt};

    printf("Functions Initialized \n");

    size_t fileCount = 0;
    FileEntry *entries = GetFileEntrys(&fileCount);

    printf(SIZE_T_IDENTIFIER " file founded", fileCount);

    for (size_t currentFileIndex = 0; currentFileIndex < fileCount; currentFileIndex++)
    {
        FileEntry currentFile = entries[currentFileIndex];

        const char* filepath = currentFile.filename;

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
            int *array = malloc(MAX_NUM_ELEMENTS * sizeof(int));
            if (!array)
            {
                perror("Memory allocation failed");
                CsvClose(handle);
                return 12;
            }

            int index = 0;
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
                array[index++] = atoi(col);
            }

            if (index == 0)
            {
                printf("Array is empty skipped");
                free(array);
                continue;
            }

            // PossibilitySpace** pbSpaces = SortV2(array, index, sizeof(int), SpaceCount, indexer, intComparer);

            for (size_t i = 0; i < numAlgorithms; i++)
            {
                benchmarkSortFunction(currentFile.id, array, index, sortingFunctions[i]);
            }

            if (useV1)
            {
                benchmarkSortFunction(currentFile.id, array, index, sortV1);
            }

            benchmarkSortFunction(currentFile.id, array, index, sortV2);

            free(array);

            count++;
        }
    }

    return 0;
}