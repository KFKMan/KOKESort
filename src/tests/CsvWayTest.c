#include "TestGlobal.h"
#include "csv.h"

int TestNotFoundFile()
{
    char* suffix = "NoFile";
    char* file = GetSuffixedTestDataFile(suffix);

    CsvHandle handle = CsvOpen(file);

    if(handle == NULL)
    {
        free(file);
        return 1;
    }

    free(handle);
    free(file);
    return 0;
}

int main(void)
{
    if(TestNotFoundFile() == 1)
    {
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}
