#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <csv.h>
#include <KOKESort.h>

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


void formatDuration(clock_t start, clock_t end) {
    double duration = (double)(end - start) / CLOCKS_PER_SEC * 1000; // milisaniye
    printf("Duration: %.3f ms\n", duration);
}

void benchmarkSortV1(const char* benchName, const int* data, size_t size) {
    int* copy = (int*)malloc(size * sizeof(int));
    memcpy(copy, data, size * sizeof(int));

    clock_t start = clock();
    SortV1Self(copy, size, intComparer, sizeof(int));
    clock_t end = clock();

    printf("%s | SortV1 | %zu --> ", benchName, size);
    formatDuration(start, end);
    free(copy);
}

void benchmarkQuickSort(const char* benchName, const int* data, size_t size) {
    int* copy = (int*)malloc(size * sizeof(int));
    memcpy(copy, data, size * sizeof(int));

    clock_t start = clock();
    qsort(copy, size, sizeof(int), intComparer);
    clock_t end = clock();

    printf("%s | QuickSort | %zu --> ", benchName, size);
    formatDuration(start, end);
    free(copy);
}

// Ana fonksiyon
int main(int argc, char** argv) {
    printf("Benchmark R2 App Started V1.0.0\n");

    const char* benchName = "Automated";
    const char* filepath = "testdata.txt";

    char* row;
    CsvHandle handle = CsvOpen2(filepath, ' ', '"', '\\');
    if (!handle) {
        printf("Failed to open file: %s\n", filepath);
        return 11;
    }

    size_t count = 0;

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
            if (index >= MAX_NUM_ELEMENTS) {
                printf("Array size exceeds maximum limit\n");
                free(array);
                CsvClose(handle);
                return 13;
            }
            array[index++] = atoi(col);
        }

        // Eğer array boşsa atla
        if (index == 0) 
        {
            printf("Array is empty skipped");
            free(array);
            continue;
        }

        printf("SortV1\n");
        benchmarkSortV1(benchName, array, index);
        
        printf("QuickSort\n");
        benchmarkQuickSort(benchName, array, index);

        free(array);
        
        count++;
    }

    return 0;
}