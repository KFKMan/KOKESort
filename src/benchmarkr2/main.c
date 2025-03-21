#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <csv.h>
#include "KOKESort.h"
#include "KOKESortV2.h"
#include "master.h"

#define MAX_NUM_ELEMENTS 10000000

int intComparer(const void* val1ptr, const void* val2ptr) {
    const int val1 = *(const int*)val1ptr;
    const int val2 = *(const int*)val2ptr;
    if (val1 > val2) {
        return 1;
    }
    if (val1 == val2) {
        return 0;
    }
    return -1;
}

const int MaxElementCount = 100000000;
const int SpaceCount = 1000;

int Divider;

size_t indexer(const void* valptr)
{
    const int val1 = *(const int*)valptr;
    return val1/Divider;
}


void formatDuration(clock_t start, clock_t end) {
    double duration = (double)(end - start) / CLOCKS_PER_SEC * 1000; // milisaniye
    printf("Duration: %.3f ms\n", duration);
}

void benchmarkSortFunction(const char* benchName, const int* data, size_t size, SortFunctionEntry sortFnEntry) {
    int* copy = (int*)malloc(size * sizeof(int));
    memcpy(copy, data, size * sizeof(int));

    printf("%s | %s - Started \n", benchName, sortFnEntry.name);
    clock_t start = clock();
    sortFnEntry.function(copy, size, sizeof(int), intComparer);
    clock_t end = clock();

    printf("%s | %s | %zu --> ", benchName, sortFnEntry.name, size);
    formatDuration(start, end);
    free(copy);
}

int GetBool(const char* question)
{
    int x = SUCCESS;
    printf(question);
    scanf("%d",&x);
    return x;
}

void SortV1SelfNonOpt(void* arr, size_t size, size_t elementSize, CompareFunction comparer)
{
    SortV1Self(arr, size, elementSize, comparer);
}

void SortV2NonOpt(void* arr, size_t size, size_t elementSize, CompareFunction comparer)
{
    PossibilitySpace** pbSpaces = SortV2(arr, size, sizeof(int), SpaceCount, indexer, intComparer);
}

// Ana fonksiyon
int main(int argc, char** argv) {
    printf("Benchmark R2 App Started V1.0.0\n");

    Divider = MaxElementCount / SpaceCount; //This is needed for Configuring Indexer Function, SortV2 using it.

    printf("Write %d for TRUE, %d for FALSE \n", SUCCESS, FAIL);
    int useV1 = GetBool("Use SortV1: ");

    const char* benchName = "Automated";
    const char* filepath = "testdata.txt";

    printf("Functions Initializing... \n");

    size_t count = 0;

    SortFunctionEntry sortingFunctions[] = _SortingFunctions;
    size_t numAlgorithms = sizeof(sortingFunctions) / sizeof(sortingFunctions[0]);

    SortFunctionEntry sortV1 = {"SortV1",SortV1SelfNonOpt};
    SortFunctionEntry sortV2 = {"SortV2",SortV2NonOpt};

    printf("Functions Initialized \n");

    printf("Opening File \n");

    char* row;
    CsvHandle handle = CsvOpen2(filepath, ' ', '"', '\\');
    if (!handle) {
        printf("Failed to open file: %s\n", filepath);
        return 11;
    }

    printf("File Opened \n");
    
    printf("Reading and Benching... \n");

    while ((row = CsvReadNextRow(handle))) {
        int* array = malloc(MAX_NUM_ELEMENTS * sizeof(int));
        if (!array) {
            perror("Memory allocation failed");
            CsvClose(handle);
            return 12;
        }

        int index = 0;
        const char* col;
        while ((col = CsvReadNextCol(row, handle))) {
            if (index >= MAX_NUM_ELEMENTS) 
            {
                printf("Array size exceeds maximum limit\n");
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

        //PossibilitySpace** pbSpaces = SortV2(array, index, sizeof(int), SpaceCount, indexer, intComparer);

        for(size_t i = 0; i < numAlgorithms; i++)
        {
            benchmarkSortFunction(benchName, array, index, sortingFunctions[i]);
        }

        if(useV1)
        {
            benchmarkSortFunction(benchName, array, index, sortV1);
        }

        benchmarkSortFunction(benchName, array, index, sortV2);

        free(array);
        
        count++;
    }

    printf("Readed and Benched \n");

    return 0;
}