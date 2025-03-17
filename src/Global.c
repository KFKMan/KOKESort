#include <Global.h>

void* GetIndex(void* arr, size_t index, unsigned int elementSize)
{
    unsigned char* ptr = (unsigned char*)arr;

    void* arrIndexPtr = ptr + index * elementSize;

    return arrIndexPtr;
}

void FreeMemory(void* pointer)
{
    free(pointer);
}