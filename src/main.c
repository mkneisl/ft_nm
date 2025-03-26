#include "main.h"

int displaySymbols(t_options* options, const char* elfFile)
{
    t_elf_map* elfMap;
    t_symbol_data* symData;

    elfMap = mapFile(elfFile);
    if (!elfMap)
        return -1;
    symData = MARCH_CALL(elfMap->arch, loadSymbols, elfMap);
    if (!symData)
    {
        unmapFile(elfMap);
        return printNoSymbols(elfFile);
    }
    if (!options->noSort)
        sortSymbols(symData, options->sortDir * -1);
    for (size_t i = 0; i < symData->symbolCount; i++){
        uint64_t symVal = 0;
        char* symName = MARCH_CALL(symData->elfMap->arch, getSymbolName, symData, symData->symbols[i]);
        int symChar = MARCH_CALL(symData->elfMap->arch, getSymbolInfo, symData, symData->symbols[i], &symVal);

        if (ft_toupper(symChar) != 'U'
            && ft_toupper(symChar) != 'W') 
            ft_printf("%c %s\n", symChar, symName);
        else
            ft_printf("%c %s\n",  symChar, symName);
    }
    unmapFile(elfMap);
    return 0;
}

int main(int argc, const char** argv)
{
    int invldFiles;
    char openDefault;
    t_options nmOption;

    if (parseArgs(&nmOption, argv) < 0)
    {
        printHelp();
        return 1;
    }
    if (nmOption.help)
        return printHelp();
    invldFiles = 0;
    openDefault = 1;
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
            continue;
        openDefault = 0;
        if (displaySymbols(&nmOption, argv[i]) < 0)
            invldFiles++;
    }
    if (!openDefault)
        return invldFiles;
    if (displaySymbols(&nmOption, "a.out") < 0)
        return 1;
    return 0;
}
