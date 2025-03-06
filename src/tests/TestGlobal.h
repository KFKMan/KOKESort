#ifndef TEST_GLOBAL_H
#define TEST_GLOBAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SecureFunctions.h"
#include "KOKESort.h"


#ifdef _WIN32
#include <direct.h> // Windows için _getcwd()
#define getcwd _getcwd
#else
#include <unistd.h> // Linux/Unix için getcwd()
#endif

/// @brief 
/// @param fp 
/// @param filename 
/// @param mode 
/// @return 0 if success, another value if failed 
int OpenFile(FILE** fp, const char* filename, const char* mode)
{
    if(fopen_s(fp, filename, mode) != 0){
        return 1;
    }
    return 0;
}

int SearchAndOpenFile(FILE** file, const char* filename, const char* mode)
{
    FILE *fp = NULL;
    int fileState = OpenFile(&fp, filename, "r");

    if (fileState != 0)
    {
        char filepath[256];
        snprintf(filepath, sizeof(filepath), "tests/%s", filename);
        fileState = OpenFile(&fp, filepath, "r");

        if (fileState != 0)
        {
            char cwd[256];
            if (getcwd(cwd, sizeof(cwd)) != NULL)
            {
                fprintf(stderr, "Working Directory: %s\n", cwd);
            }
            return 1;
        }
    }
    *file = fp;
    return 0;
}

const char* ChangeFileExtension(const char* fileName, const char* extension) 
{
    static char newFileName[256];
    strcpy_s(newFileName, 256, fileName);
    char* dotPos = strrchr(newFileName, '.');
    if (dotPos) 
    {
        *dotPos = '\0'; // Uzantıyı sil
    }
    strcat_s(newFileName, 256, extension); // Yeni uzantıyı ekle
    return newFileName;
}

#define TryOpenTestDataFile(file, mode) SearchAndOpenFile(file, ChangeFileExtension(__FILE__, ".txt"), mode)

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
int ArrayEqual(int* arr, int* otherArr, size_t size)
{
    for(size_t i = 0; i < size; i++)
    {
        if(arr[i] != otherArr[i])
        {
            return 1;
        }
    }
    return 0;
}

void PrintArray(int* arr, size_t size)
{
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


