#ifndef BENCHMARKR2_FILE_H
#define BENCHMARKR2_FILE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Global.h"

#ifdef WINDOWS
    #include <windows.h>
#else
    #include <dirent.h>
#endif

#define R2_MAX_NUM_FILES 100000
#define R2_MAX_FILENAME 2048

int file_exists(const char* filename);

typedef struct {
    char filename[R2_MAX_FILENAME];
    long long id;
} FileEntry;

long long extract_id(const char *filename);

FileEntry* GetFileEntrys(size_t* numberOfFiles);

#endif