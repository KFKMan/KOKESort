#include "file.h"

int file_exists(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file) 
    {
        fclose(file);
        return TRUE; 
    }
    return FALSE;
}

int extract_id(const char *filename) 
{
    if (strncmp(filename, "test", 4) != 0) 
    {
        return -1;
    }

    const char *start = filename + 4;
    char *end;
    int id = strtol(start, &end, 10);

    if (id < 0 || strcmp(end, ".csv") != 0) 
    {
        return -1;
    }
    return id;
}

FileEntry* GetFileEntrys(size_t* numberOfFiles)
{
    FileEntry* files = malloc(sizeof(FileEntry) * R2_MAX_NUM_FILES);
    size_t count = 0;

#ifdef _WIN32
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile("test*.csv", &findFileData);

    if (hFind != INVALID_HANDLE_VALUE) 
    {
        do 
        {
            int id = extract_id(findFileData.cFileName);
            if (id != -1) 
            {
                strncpy(files[count].filename, findFileData.cFileName, sizeof(files[count].filename) - 1);
                files[count].filename[sizeof(files[count].filename) - 1] = '\0';
                files[count].id = id;
                count++;
            }
        } while (FindNextFile(hFind, &findFileData) != 0);
        FindClose(hFind);
    }
#else
    DIR *dir = opendir(".");
    if (!dir) {
        perror("opendir");
        return 1;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        int id = extract_id(entry->d_name);
        if (id != -1) {
            strncpy(files[count].filename, entry->d_name, sizeof(files[count].filename) - 1);
            files[count].filename[sizeof(files[count].filename) - 1] = '\0';
            files[count].id = id;
            count++;
        }
    }
    closedir(dir);
#endif

    *numberOfFiles = count;
    return files;
}