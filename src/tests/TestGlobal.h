#ifndef TEST_GLOBAL_H
#define TEST_GLOBAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SecureFunctions.h"
#include "KOKESort.h"


#ifdef _WIN32
#include <direct.h> // For Windows _getcwd()
#define getcwd _getcwd
#else
#include <unistd.h> // For Linux/Unix getcwd()
#endif

#define MAX_FILE_PATH_SIZE 4096

/// @brief 
/// @param fp 
/// @param filename 
/// @param mode 
/// @return 0 if success, another value if failed 
int OpenFile(FILE** fp, const char* filename, const char* mode)
{
    if(fopen_s(fp, filename, mode) != 0)
    {
        return 1;
    }
    return 0;
}

int SearchAndOpenFile(FILE** file, const char* filename, const char* mode)
{
    const char* directories[] = {".", "tests", "benchmark", "example"};
    const size_t dirCount = sizeof(directories) / sizeof(directories[0]);

    FILE *fp = NULL;
    char filepath[MAX_FILE_PATH_SIZE];

    // Try opening the file in each directory
    for (size_t i = 0; i < dirCount; ++i)
    {
        snprintf(filepath, sizeof(filepath), "%s/%s", directories[i], filename);

        // Try opening the file
        int fileState = OpenFile(&fp, filepath, mode);
        if (fileState == 0)
        {
            *file = fp;
            return 0;  // Success, file opened
        }
    }

    // If file wasn't found, print the current working directory
    char cwd[MAX_FILE_PATH_SIZE];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        fprintf(stderr, "File not found in directories. Working Directory: %s\n", cwd);
    }
    else
    {
        fprintf(stderr, "Error getting current working directory.\n");
    }
    
    return 1;  // Failed to open the file
}

const char* GetFilename(const char* path)
{
    const char *filename = strrchr(path, '/');  // Find the last occurrence of '/'

    if (filename == NULL) 
    {
        filename = strrchr(path, '\\');  // If '/' is not found, try '\' -> For Windows
    }

    if (filename != NULL) 
    {
        return filename + 1;  // Skip the '/' character to get the filename
    } 
    else 
    {
        return path;  // Return the original path if no '/' is found
    }
}

const char* ChangeFileExtension(const char* fileName, const char* extension) 
{
    static char newFileName[MAX_FILE_PATH_SIZE];
    strcpy_s(newFileName, MAX_FILE_PATH_SIZE, fileName);
    char* dotPos = strrchr(newFileName, '.');
    if (dotPos) 
    {
        *dotPos = '\0'; // Removing the extension string to empty
    }
    strcat_s(newFileName, MAX_FILE_PATH_SIZE, extension); // Adding new extension string
    return newFileName;
}

#define TryOpenTestDataFile(file, mode) SearchAndOpenFile(file, ChangeFileExtension(GetFilename(__FILE__), ".txt"), mode)

size_t ReadArray(int* arr, char* str, const char* delimiter, char** context, size_t limit)
{
    size_t count = 0;
    char *token = strtok_s(str, delimiter, context);
    while (token && count < limit)
    {
        arr[count++] = atoi(token);
        token = strtok_s(NULL, delimiter, context);
    }

    return count;
}

/// @brief 
/// @param arr 
/// @param otherArr 
/// @param size 
/// @return 0 if equal, other value if not equal 
int ArrayEqual(void* arr, void* otherArr, size_t size, CompareFunction comparer, unsigned int elementSize)
{
    for(size_t i = 0; i < size; i++)
    {
        if(comparer(GetIndex(arr,i,elementSize), GetIndex(otherArr, i, elementSize)) != 0)
        {
            return 1;
        }
    }
    return 0;
}

void PrintArray(void* Varr, size_t size)
{
    int* arr = Varr;
    printf("[" SIZE_T_IDENTIFIER " - ", size);
    printf("(");
    for (size_t i = 0; i < size; i++)
    {
        printf("%d", arr[i]);
        if (i < size - 1)
        {
            printf(", ");
        }
    }
    printf(")");
    printf("]");
}

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

#endif


