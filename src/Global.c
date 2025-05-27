#include "Global.h"

int TestFunction(void)
{
    return 1;
}

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

int GetPointerAllocationType(void* ptr)
{
    #ifdef WINDOWS
    MEMORY_BASIC_INFORMATION mbi;
    VirtualQuery(ptr, &mbi, sizeof(mbi));
    
    if (mbi.Type == MEM_PRIVATE && mbi.AllocationBase == ptr)
    {
        return GVIRTUAL;
    }
    #else
    FILE *fp = fopen("/proc/self/maps", "r");
    if (!fp) {
        perror("fopen");
        return GRAM;
    }

    unsigned long address = (unsigned long)ptr;
    char line[256];

    while (fgets(line, sizeof(line), fp)) 
    {
        unsigned long start, end;
        if (sscanf(line, "%lx-%lx", &start, &end) == 2) 
        {
            if (address >= start && address < end) 
            {
                printf("Pointer is in memory region: %s", line);
                fclose(fp);
                return GVIRTUAL;
            }
        }
    }

    fclose(fp);
    #endif

    return GRAM;
}

void* AllocFn(size_t size)
{
    void* fn = AllocNonVirtualFn(size);
    if(fn == NULL)
    {
        fn = AllocVirtualFn(size);
    }
    return fn;
}

void* AllocNonVirtualFn(size_t size)
{
    return malloc(size);
}

void* AllocVirtualFn(size_t size)
{
    void* fn = NULL;
    #ifdef WINDOWS
        fn = VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    #else
        fn = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

        if(fn == MAP_FAILED)
        {
            return NULL;
        }
    #endif

    return fn;
}

void DeAllocFn(void* ptr, size_t size)
{
    int mode = GetPointerAllocationType(ptr);
    if(mode == GRAM)
    {
        DeAllocNonVirtualFn(ptr);
    }
    else
    {
        DeAllocVirtualFn(ptr, size);
    }
}

void DeAllocNonVirtualFn(void* ptr)
{
    free(ptr);
}

void DeAllocVirtualFn(void* ptr, size_t size)
{
    #ifdef WINDOWS
        VirtualFree(ptr, 0, MEM_RELEASE);
    #else
        munmap(ptr, size);
    #endif
}