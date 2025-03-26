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
    sortSymbols(symData, 1);
    for (size_t i = 0; i < symData->symbolCount; i++){
        uint64_t symVal = 0;
        char* symName = MARCH_CALL(symData->elfMap->arch, getSymbolName, symData, symData->symbols[i]);
        int symChar = MARCH_CALL(symData->elfMap->arch, getSymbolInfo, symData, symData->symbols[i], &symVal);
        // if (!ft_strlen(symName))
        //     continue;
        // if (ft_toupper(symChar) == 'A')
        //     continue;
        if (ft_toupper(symChar) != 'U'
            && ft_toupper(symChar) != 'W') 
            printf("%016lx %c %s\n", symVal, symChar, symName);
        else
            printf("%16s %c %s\n", "", symChar, symName);
    }
    unmapFile(elfMap);
    return 0;
}
