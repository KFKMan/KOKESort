#include "TestGlobal.h"
#include "csv.h"

int ReadVariables(int* expRow, int* expColumn)
{
    char* suffix = ".Expect";
    char* suffixedFile = GetSuffixedTestDataFile(suffix);
    printf("Prefixed File %s \n", suffixedFile);

    CsvHandle reader = CsvOpen2(suffixedFile, ';', '"', '\\'); 
    if(!reader)
    {
        return 1;
    }

    char* readerRow = CsvReadNextRow(reader);
    const char* readerCol = NULL;

    if(!(readerCol = CsvReadNextCol(readerRow, reader)))
    {
        return 1;
    }

    *expRow = atoi(readerCol);

    if(!(readerCol = CsvReadNextCol(readerRow, reader)))
    {
        return 1;
    }

    *expColumn = atoi(readerCol);

    CsvClose(reader);

    free(suffixedFile);

    return 0;
}

int main(void)
{
    int rowcount = 0;
    int colcount = 0;
    char* row = NULL;
    int expRows = 0;
    int expCols = 0;

    if(ReadVariables(&expRows, &expCols) == 1)
    {
        perror("Variable read error");
        return EXIT_FAILURE;
    }


    CsvHandle handle = CsvOpen(GetTestDataFile());
    if (!handle)
    {
        perror("CsvOpen error");
        return EXIT_FAILURE;
    }

    while ((row = CsvReadNextRow(handle)))
    {
        const char* col = NULL;
        rowcount++;
        while ((col = CsvReadNextCol(row, handle)))
        {
            colcount++;
        }
    }

    CsvClose(handle);
    /* expectations */
    printf("Row Count: %d/%d, Total Column Count: %d/%d\n",
           rowcount, expRows, colcount, expCols);
    
    return (expRows == rowcount && expCols == colcount) ? EXIT_SUCCESS : EXIT_FAILURE;
}
