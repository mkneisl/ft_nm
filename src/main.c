#include "main.h"

int main(int argc, char** argv)
{
    char* fileName;
    t_elf_map* elfMap;
    t_symbol_data* symData;

    if (argc < 2)
        fileName = "a.out";
    else
        fileName = argv[1];
    elfMap = mapFile(fileName);
    if (!elfMap)
    {
        printf("Cant map file\n");
        return 1;
    }
    symData = MARCH_CALL(elfMap->arch, loadSymbols, elfMap);
    if (!symData)
    {
        printf("No symbols\n");
        return 1;
    }
    for (size_t i = 0; i < symData->symbolCount; i++)
        printf("->%s\n", getSymbolName(symData, symData->symbols[i]));
    unmapFile(elfMap);
    return 0;
}
