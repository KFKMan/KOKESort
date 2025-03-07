#include "ubench.h"
#include "KOKESort.h"

//#define DEBUG

UBENCH_STATE();

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


UBENCH_EX(Sort, int) 
{  
    //Setup Data etc etc
    int arr[] = {6041, 525, 1251, 3272, 1199, 6079, 3013, 2999, 6035, 1566, 1250, 8894, 1471, 2190, 7110, 7301, 1650, 8368, 1085, 1045, 6208, 8590, 8846, 3186, 5109, 5173, 4220, 725, 6697, 7852};
    int* sortedArr = NULL;

    UBENCH_DO_BENCHMARK() 
    {
        sortedArr = SortV1(arr, sizeof(arr)/sizeof(int), intComparer);
    }

    free(sortedArr);
}


int main(int argc, const char *const argv[]) 
{
    return ubench_main(argc, argv);
}