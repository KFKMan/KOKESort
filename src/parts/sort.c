#include "sort.h"

void swap(void* firstEl, void* secondEl, size_t size)
{
    //TODO: It's not a effective solution, change it.
    char *pa = (char *)firstEl;
    char *pb = (char *)secondEl;
    for (size_t i = 0; i < size; i++) 
    {
        char temp = pa[i];
        pa[i] = pb[i];
        pb[i] = temp;
    }
}